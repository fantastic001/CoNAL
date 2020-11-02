
#include <CodeManagerComponent.hpp>
#include <iostream>
#include <sstream>

using namespace conal::code_manager; 
using namespace conal::framework;

CodeManagerComponent::CodeManagerComponent() {
}
void CodeManagerComponent::start() {
    logger->info("Starting Code manager component");
    
}

void CodeManagerComponent::stop() {

}

void CodeManagerComponent::handleMessage(Message msg) {
    logger->debug("Got message: " + msg.body);
    if (msg.performative == Performative::REQUEST) {
        std::stringstream ss(msg.body);
        std::string command; 
        ss >> command;
        if (command == "load") {
            std::string path;
            ss >> path;
            std::vector<std::string> params; 
            while (ss.good()) {
                std::string param; 
                ss >> param;
                params.push_back(param);
            }
            logger->info("Loading code " + path);
            for (auto p : params) {
                logger->debug(p);
            }
        }
    }
}
