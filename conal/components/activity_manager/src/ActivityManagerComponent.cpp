
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
        clientThread = std::thread(&ActivityManagerComponent::runClient, this);
        clientThread.detach();
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
    [this, logger = logger] (std::shared_ptr<Connection> conn, std::string message) {
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
        else if (command == "HI") {
            logger->debug("Got HI from " + conn->getHostname());
        }
        else if (command == "COMPONENT_REPLY") {
            logger->debug("Parsing message");
            Message msg; 
            ss >> msg;
            stringstream resultWriter;
            resultWriter << msg; 
            logger->debug("Message parsed: " + resultWriter.str());
            sendFromClient(conn->getHostname(), msg);
        }
        
    });
}

void ActivityManagerComponent::runClient() {
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

void ActivityManagerComponent::handleMessage(Message msg) {
    logger->debug("Got message: " + msg.body + " from " + msg.from_component);
    if (msg.performative == Performative::REPLY 
        && waitingFromComponent.find(msg.reply_with) != waitingFromComponent.end()
    ) {
        // change reply_with with number sent from server
        logger->debug("Got reply from local component, sending back to master");
        logger->debug("Sending to needed connection");
        waitingFromComponent[msg.reply_with].set_value(msg);
        logger->debug("Future set");
    } 
    if (msg.performative == Performative::REQUEST) {
        string command; 
        stringstream ss(msg.body);
        ss >> command;
        if (command == "list") {
            auto selection = msg.body.substr(5);
            node_spec::Parser p; 
            auto specParsed = p.parse(selection.c_str());
            logger->info("Listing all connections");
            connectionManager.removeClosed();
            logger->debug("Removed all closed connections");
            auto connections = connectionManager.select(specParsed);
            stringstream reply;
            int i = 1;
            for (auto iter = connections.begin(); iter != connections.end(); iter++) {
                if (msg.from_component == "console") {
                    std::cout 
                        << "Client " 
                        << i << ": "
                        << (*iter)->getHostname() 
                        << " NAME=" << (*iter)->getProperty("NAME")
                        << " ARCH=" << (*iter)->getProperty("ARCH")
                        << std::endl;
                    i++;
                }
                else {
                    reply << (*iter)->getHostname() << " ";
                }
            }
            if (msg.from_component != "console") {
                this->reply(msg, reply.str());
            }
        }
        else if (command == "send")
        {
            auto rest = msg.body.substr(5); // "send ____________..."
            int separatorIndex = rest.find_first_of('#');
            auto selection = rest.substr(0, separatorIndex);
            auto command = rest.substr(separatorIndex+1);
            node_spec::Parser p;
            auto selParsed = p.parse(selection.c_str());
            auto connections = connectionManager.select(selParsed);
            logger->debug("Sending command to clients: " + selParsed->dump());
            for (auto conn : connections) {
                conn->send(command);
            }
        }
        else if (command == "send_message") {
            string selection;
            Message msg_to_be_sent; 
            ss >> selection;
            ss >> msg_to_be_sent;
            msg_to_be_sent.reply_with = msg.reply_with;
            stringstream msgWriter;
            msgWriter << " " << msg_to_be_sent;
            node_spec::Parser parser; 
            auto selParsed = parser.parse(selection.c_str());
            waitingFromClients[msg_to_be_sent.reply_with] = msg_to_be_sent;
            for (auto conn : connectionManager.select(selParsed)) {
                logger->debug("Sending message to " + conn->getHostname() + ": " + msgWriter.str());
                conn->send("COMPONENT " + msgWriter.str());
            }

        }
    }
    else if (msg.from_component == "code_manager") handleMessageFromCodeManager(msg);
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
std::string ActivityManagerComponent::clearDataSpecification(std::string spec) {
    while (spec[0] == ' ') spec.erase(0, 1);
    return spec;
}
void ActivityManagerComponent::handleMessageFromDataManager(Message msg) {
}

void ActivityManagerComponent::handleMessageFromUser(Message msg) {
   if (msg.performative == Performative::CREATE) {
        std::unique_lock<std::mutex> lock(task_mutex);
        std::string selection; 
        std::stringstream reader(msg.body);
        stringstream codeManagerBodyWriter;
        reader >> selection;
        std::string path; 
        std::vector<std::string> params;
        reader >> path;
        codeManagerBodyWriter << path;
        logger->info("Requested to run program: " + path);
        while (reader.good()) {
            std::string param; 
            reader >> param;
            codeManagerBodyWriter << " " << param;
            params.push_back(param);
        }
        tasks.push_back(Task(path, params));
        auto& task = tasks.back();
        task.setState(TaskState::PREPARING);
        auto id = tasks.size()-1;
        for (auto connection : connectionManager.select(node_spec::Parser().parse(selection.c_str()))) {
            std::stringstream ss;
            ss << id << " ";
            ss << connection->getHostname() << " ";
            ss << "ARCH=" << connection->getProperty("ARCH") << " ";
            ss << codeManagerBodyWriter.str();
            task.addConnection(connection);
            logger->info("Sending parameters for code manager for task " + ss.str());
            sendMessage(
                "code_manager", 
                Performative::REQUEST, 
                ss.str());
        }
    }
}
bool ActivityManagerComponent::isSlave() const {
    return ! masterHostname.empty();
}

void ActivityManagerComponent::handleClientReply(std::shared_ptr<::conal::framework::TCPClient> conn, std::string reply) {
        std::string command; 
        logger->debug("Got from master: " + reply);
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
            string specification = reply.substr(5);
            sendMessage("data_manager", Performative::CREATE, specification);
        }
        else if (command == "HI") {
            logger->debug("Got HI from master!");
            conn->send("HI");
        }
        else if (command == "COMPONENT") {
            logger->debug("Parsing message...");
            Message message;
            ss >> message;
            logger->debug("Sending message from server to local component " + message.to_component);

            
            std::promise<Message> my_promise;
            auto my_future = my_promise.get_future();

            int reply_with = sendReplyableMessage(
                message.to_component, 
                message.performative, 
                message.body
            );
            waitingFromComponent[reply_with] = std::move(my_promise);
            logger->debug("Waiting for future");
            auto reply = my_future.get();
            reply.reply_with = message.reply_with;
            logger->debug("Future getting unblocked");
            stringstream replyWriter;
            replyWriter << reply;
            conn->send("COMPONENT_REPLY " + replyWriter.str());
            logger->debug("sent!");
            waitingFromComponent.erase(reply_with);
        }

}

void ActivityManagerComponent::sendFromClient(string hostname, Message message) {
    if (waitingFromClients.find(message.reply_with) != waitingFromClients.end()) {
        logger->debug("Found waiting delivery");
        if (waitingFromClients[message.reply_with].from_component == "console") {
            std::cout << "From " << hostname << ": " << message.body << "\n";
        }
        else reply(waitingFromClients[message.reply_with], 
        hostname + " " + message.body, message.performative);
    }
}