
#include <ActivityManagerComponent.hpp>
#include <iostream>
#include <TCPServer.hpp>
#include <Connection.hpp>
#include <sstream>

using namespace conal::activity_manager; 
using namespace conal::framework;

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
}

bool ActivityManagerComponent::isSlave() const {
    return ! masterHostname.empty();
}