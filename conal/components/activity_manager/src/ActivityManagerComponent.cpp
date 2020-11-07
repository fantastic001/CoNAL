
#include <ActivityManagerComponent.hpp>
#include <iostream>
#include <TCPServer.hpp>
#include <Connection.hpp>
#include <sstream>
#include <sstream>

using namespace conal::activity_manager; 
using namespace conal::framework;
using namespace std;

ActivityManagerComponent::ActivityManagerComponent() {
}
void ActivityManagerComponent::start() {
    logger->info("Starting activity manager component");
    masterHostname = std::string(getenv("CONAL_MASTER_HOSTNAME"));
    if (isSlave()) {
        logger->info("Connecting to server...");
        clientPtr = std::make_shared<TCPClient>(masterHostname, "6969");
        logger->info("Client connected");
        clientPtr->send("HELLO");
        logger->debug("Data successfully written to pipe");
        std::string reply = clientPtr->readLine();
        if (reply == "HELLO2") {
            logger->info("Handshake done!");
            logger->debug("Sending all parameters for client");
            clientPtr->send(std::string("SET ARCH ") + std::string(getenv("CONAL_HOST_PLATFORM")));
            clientPtr->send(std::string("SET NAME ") + std::string(getenv("CONAL_CLIENT_NAME")));
        }
        else {
            logger->error("Did not get reply to handshake");
        }
    }
    else {
        logger->info("Running in master mode");
        server = std::make_shared<TCPServer>("0.0.0.0", 6969);
        serverThread = std::thread(&ActivityManagerComponent::runServer, this);
        serverThread.detach();
        logger->info("Server initialized");
    }
}

void ActivityManagerComponent::stop() {

}

void ActivityManagerComponent::runServer() {
    server->run([logger = logger, &manager = connectionManager] (std::shared_ptr<Connection> conn) {
        logger->info("Adding connection " + conn->getHostname());
        manager.addConnection(conn);
    },
    [logger = logger] (std::shared_ptr<Connection> conn, std::string message) {
        logger->debug("Received message: " + message);
        std::stringstream ss(message); 
        std::string command; 
        ss >> command; 

        if (command == "SET") {
            std::string paramName, value; 
            ss >> paramName; 
            ss >> value;
            logger->debug("Setting " + paramName + " to " + value + " for " + conn->getHostname());
            conn->setProperty(paramName, value);
        }
        else if (command == "HELLO") {
            conn->send("HELLO2");
        }
        else if (command == "CODE") {
            int id; 
            std::string code; 
            ss >> id;
            ss >> code;
            // Create Task here and assign code to it
        }
        else if (command == "READY") {
            int id; 
            std::string code; 
            ss >> id;
            // if all have been received and everything is okay, send starting commands
        }
        else if (command == "STARTED") {
            int id; 
            std::string code; 
            ss >> id;
            ss >> code;
            // Create Task here and assign code to it
        }
        // CLIEN ONLY
        else if (command == "CODE") {
            int id; 
            std::string code; 
            ss >> id;
            ss >> code;
            // Create Task here and assign code to it
        }
        else if (command == "START") {
            int id; 
            ss >> id;
            // CLIENT SIDE
            // Send code to code manager for running it
        }

    });
}

void ActivityManagerComponent::handleMessage(Message msg) {
    logger->debug("Got message: " + msg.body);
    if (msg.performative == Performative::REQUEST) {
        if (msg.body == "list") {
            logger->info("Listing all connections");
            connectionManager.removeClosed();
            logger->debug("Removed all closed connections");
            int i = 1;
            auto connections = connectionManager.getConnections();
            for (auto iter = connections.begin(); iter != connections.end(); iter++) {
                std::cout 
                    << "Client " 
                    << i << ": "
                    << (*iter)->getHostname() 
                    << " NAME=" << (*iter)->getProperty("NAME")
                    << " ARCH=" << (*iter)->getProperty("ARCH")
                    << std::endl;
                i++;
            }
        }
    }
    else if (msg.performative == Performative::CREATE) {
        std::unique_lock<std::mutex> lock(task_mutex);
        std::string body = msg.body;
        std::stringstream reader(msg.body);
        std::string path; 
        std::vector<std::string> params;
        reader >> path;
        while (reader.good()) {
            std::string param; 
            reader >> param;
            params.push_back(param);
        }
        tasks.push_back(Task(path, params));
        auto task = tasks.back();
        task.setState(TaskState::PREPARING);
        auto id = tasks.size()-1;
        for (auto connection : connectionManager.getConnections()) {
            std::stringstream ss;
            ss << id << " ";
            ss << connection->getHostname() << " ";
            ss << "ARCH=" << connection->getProperty("ARCH");
            task.addConnection(connection);
            logger->info("Sending parameters for code manager for task " + ss.str());
            sendMessage(
                "code_manager", 
                Performative::REQUEST, 
                ss.str() + " " + body);
        }
        task.setState(TaskState::PREPARED);
    }
    else if (msg.performative == Performative::ACCEPT) {
        std::string body = msg.body;
        std::unique_lock<std::mutex> lock(task_mutex);
        stringstream ss(msg.body);
        int id; 
        std::string hostname;
        ss >> id; 
        ss >> hostname;
        for (auto conn : connectionManager.getConnections()) {
            if (conn->getHostname() == hostname) {
                tasks[id].prepare(conn);
            }
        }
        if (tasks[id].isPrepared()) {
            tasks[id].setState(TaskState::LOADING);
            for (auto connection : tasks[id].getConnections()) {
                sendMessage(
                    "code_manager",
                    Performative::DATA,
                    body
                );
            }
        }
    }
    else if (msg.performative == Performative::DATA) {
        std::unique_lock<std::mutex> lock(task_mutex);
        std::stringstream ss(msg.body);
        int id; 
        ss >> id;
        std::string hostname; 
        ss >> hostname;
        std::string code;
        ss >> code;
        tasks[id].setState(TaskState::LOADING);
        for (auto cc : connectionManager.getConnections()) {
            if (cc->getHostname() == hostname) {
                tasks[id].load(cc, code);
            }
        }
        if (tasks[id].isLoadable()) {
            stringstream idWriter;
            tasks[id].setState(TaskState::SENDING);
            idWriter << id;
            for (auto c : tasks[id].getConnections()) {
                c->send("CODE " + idWriter.str() + " " + tasks[id].getCode(c));
            }
            tasks[id].setState(TaskState::SENT);
        }
    }
}

bool ActivityManagerComponent::isSlave() const {
    return ! masterHostname.empty();
}