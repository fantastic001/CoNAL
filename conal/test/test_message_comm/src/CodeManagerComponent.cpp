
#include "CodeManagerComponent.hpp"
#include <iostream>
#include <sstream>

using namespace conal::code_manager; 
using namespace conal::framework;

CodeManagerComponent::CodeManagerComponent(bool ping) : ping(ping) {
}
void CodeManagerComponent::start() {
    logger->info("Starting pingpong test component");
    if (ping) {
        logger->info("Sending message to pong");
        sendMessage("pong", Performative::REQUEST, "ping");
    }
}

void CodeManagerComponent::stop() {

}

void CodeManagerComponent::handleMessage(Message msg) {
    logger->debug("Got message: " + msg.body);
    if (!ping) {
        if (msg.performative == Performative::REQUEST && msg.body == "ping") {
            sendMessage("ping", Performative::REQUEST, "pong");
        }
    }
    else {
        if (msg.performative == Performative::REQUEST && msg.body == "pong") {
            logger->info("TEST PASSED");
        }
    }
}
