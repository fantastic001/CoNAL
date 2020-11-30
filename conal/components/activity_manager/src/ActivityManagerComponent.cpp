
#include <ActivityManagerComponent.hpp>
#include <iostream>
#include <TCPServer.hpp>
#include <Connection.hpp>
#include <sstream>

#include <node_spec/Parser.hpp>

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
        do {
            reply = clientPtr->readLine();
            handleClientReply(clientPtr, reply);
        } while (reply != "END");
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
        else if (command == "READY") {
            int id; 
            std::string code; 
            std::string loaderName; 
            ss >> id;
            ss >> code; 
            ss >> loaderName;
            std::stringstream idWriter;
            idWriter << id; 
            if (!code.empty()) {
                conn->send("START " + idWriter.str() + " " + code + " " + loaderName);
            }
        }
        else if (command == "STARTED") {
            int id; 
            std::string code; 
            ss >> id;
            ss >> code;
            logger->info(message);
        }
        
    });
}

void ActivityManagerComponent::handleMessage(Message msg) {
    logger->debug("Got message: " + msg.body + " from " + msg.from_component);
    if (msg.from_component == "code_manager") handleMessageFromCodeManager(msg);
    else if (msg.from_component == "data_manager") handleMessageFromDataManager(msg);
    else handleMessageFromUser(msg);
}

void ActivityManagerComponent::handleMessageFromCodeManager(Message msg) {
    if (msg.performative == Performative::ACCEPT) {
        std::string body = msg.body;
        logger->debug("Accepted for " + body);
        std::unique_lock<std::mutex> lock(task_mutex);
        stringstream ss(msg.body);
        int id; 
        std::string hostname;
        ss >> id; 
        ss >> hostname;
        std::stringstream idWriter;
        idWriter << id;
        logger->debug("Setting task state for particular connection");
        for (auto conn : connectionManager.getConnections()) {
            if (conn->getHostname() == hostname) {
                logger->debug("CN=" + conn->getHostname());
                tasks[id].prepare(conn);
            }
        }
        logger->debug("Checking if connections are prepared");
        if (tasks[id].getState() == TaskState::PREPARED) {
            logger->debug("Ready to request code");
            tasks[id].setState(TaskState::LOADING);
            for (auto connection : tasks[id].getConnections()) {
                sendMessage(
                    "code_manager",
                    Performative::DATA,
                    idWriter.str() + " " + connection->getHostname()
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
        std::string loaderName;
        ss >> loaderName;
        tasks[id].setState(TaskState::LOADING);
        for (auto cc : connectionManager.getConnections()) {
            if (cc->getHostname() == hostname) {
                tasks[id].load(cc, code);
            }
        }
        if (tasks[id].getState() == TaskState::LOADED) {
            stringstream idWriter;
            tasks[id].setState(TaskState::SENDING);
            idWriter << id;
            for (auto c : tasks[id].getConnections()) {
                logger->debug("Compiled for " + c->getHostname() + " id=" + idWriter.str() + " using loader " + loaderName);
                c->send("CODE " + idWriter.str() + " " + tasks[id].getCode(c) + " " + loaderName);

            }
            tasks[id].setState(TaskState::SENT);
        }
    }
}

void ActivityManagerComponent::handleMessageFromDataManager(Message msg) {
    if (msg.performative == Performative::DATA) {
        stringstream reader(msg.body);
        int id; 
        string name; 
        string specification; 
        reader >> id >> name; 
        getline(reader, specification);
        logger->info("Activity manager got information that data source is created in local");
        logger->debug("Specification: " + specification);
        if (dataIdToSourceNameMapping.find(id) != dataIdToSourceNameMapping.end()) {
            logger->warning("This id is already found, maybe it was id of old data source.");
        }
        dataSpecToIdMapping[specification] = id;
        dataIdToSourceNameMapping[id] = name; 
        if (!isSlave()) {
            for (auto conn : dataSpecToConnectionsMapping[specification]) {
                logger->debug("Requesting data creation from: " + conn->getHostname());
                conn->send("DATA " + specification);
            }
        }


    }
}

void ActivityManagerComponent::handleMessageFromUser(Message msg) {
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
        logger->info("Requested to run program: " + path);
        while (reader.good()) {
            std::string param; 
            reader >> param;
            params.push_back(param);
        }
        tasks.push_back(Task(path, params));
        auto& task = tasks.back();
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
    }
    else if (msg.performative == Performative::DATA) {
        int sepIndex = msg.body.find_first_of('|');
        string specification = msg.body.substr(0, sepIndex);
        string selection = msg.body.substr(sepIndex + 1, msg.body.size());
        logger->debug("Specification: " + specification);
        logger->debug("Selection: " + selection);
        auto parsedSpecification = node_spec::Parser().parse(specification.c_str());
        dataSpecToConnectionsMapping[parsedSpecification->dump()] 
            = connectionManager.select(parsedSpecification);
        sendMessage("data_manager", Performative::CREATE, specification);
    }
}
bool ActivityManagerComponent::isSlave() const {
    return ! masterHostname.empty();
}

void ActivityManagerComponent::handleClientReply(std::shared_ptr<::conal::framework::TCPClient> conn, std::string reply) {
        std::string command; 
        std::stringstream ss(reply);
        ss >> command;
        if (command == "CODE") {
            int id; 
            std::stringstream idWriter;
            std::string code; 
            ss >> id;
            ss >> code;
            idWriter << id;
            // Create Task here and assign code to it
            clientTaskIdToCodeMapping[id] = code;
            std::string loaderName;
            ss >> loaderName;
            clientTaskIdToLoaderMapping[id] = loaderName;
            logger->debug("Ready to run!");
            conn->send("READY " + idWriter.str() + " " + code + " " + loaderName);

        }
        else if (command == "START") {
            int id; 
            ss >> id;
            string loaderName = clientTaskIdToLoaderMapping[id];
            string code = clientTaskIdToCodeMapping[id];
            sendMessage("code_manager", Performative::START, loaderName + " " + code);
        }
        else if (command == "DATA") {
            string specification; 
            getline(ss, specification);
            sendMessage("data_manager", Performative::CREATE, specification);
        }

}
