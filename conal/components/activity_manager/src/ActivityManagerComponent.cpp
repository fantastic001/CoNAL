
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
    server = std::make_shared<TCPServer>("0.0.0.0", 6969);
    serverThread = std::thread(&ActivityManagerComponent::runServer, this);
    serverThread.detach();
    logger->info("Server initialized");
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
                std::cout << "Client " << i << ": "<< (*iter)->getHostname() << std::endl;
                i++;
            }
        }
    }
}