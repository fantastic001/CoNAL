
#include <ActivityManagerComponent.hpp>
#include <iostream>
#include <TCPServer.hpp>
#include <Connection.hpp>

using namespace conal::activity_manager; 
using namespace conal::framework;

ActivityManagerComponent::ActivityManagerComponent() {
}

void ActivityManagerComponent::start() {
    server = std::make_shared<TCPServer>("0.0.0.0", 6969);
    serverThread = std::thread(&ActivityManagerComponent::runServer, this);
    serverThread.detach();
}

void ActivityManagerComponent::stop() {

}

void ActivityManagerComponent::runServer() {
    server->run([&manager = connectionManager] (std::shared_ptr<Connection> conn) {
        manager.addConnection(conn);
    },
    [] (std::shared_ptr<Connection> conn, std::string message) {
        conn->send(message + "\n");
    });
}

void ActivityManagerComponent::handleMessage(Message msg) {
    std::cout << "Got message " << msg.body << std::endl;
    if (msg.performative == Performative::REQUEST) {
        if (msg.body == "list") {
            // int i = 1;
            // for (auto iter = connections.begin(); iter != connections.end(); iter++) {
            //     std::cout << "Client: " << i << (*iter)->getHostname() << std::endl;
            //     i++;
            // }
        }
    }
}