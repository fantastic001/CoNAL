
#include <ActivityManagerComponent.hpp>
#include <iostream>
#include <TCPServer.hpp>
#include <Connection.hpp>

using namespace conal::activity_manager; 
using namespace conal::framework;

void ActivityManagerComponent::start() {
    server = std::make_shared<TCPServer>("0.0.0.0", 6969);
    serverThread = std::thread(&ActivityManagerComponent::runServer, this);
    serverThread.detach();
}

void ActivityManagerComponent::stop() {

}

void ActivityManagerComponent::runServer() {
    server->run([] (Connection& conn) {
        conn.send("hello\n");
    },
    [] (Connection& conn, std::string message) {
        conn.send(message + "\n");
    });
}

void ActivityManagerComponent::handleMessage(Message msg) {
    std::cout << "Got message " << msg.body << std::endl;
}