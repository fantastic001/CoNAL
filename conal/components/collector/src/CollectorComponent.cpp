
#include "CollectorComponent.hpp"
#include <iostream>
#include <sstream>
#include <Performative.hpp>

using namespace conal::collector; 
using namespace conal::framework;

CollectorComponent::CollectorComponent(int argc, char** argv)  : Component() {
    if (argc < 3) {
        std::cout << "Wrong number of arguments!\n";
        finish();
        return; 
    }
    selection = std::string(argv[1]);
    to = std::string(argv[2]);
    perStr = std::string(argv[3]);
    message = "";
    for (int i = 4; i<argc; i++) {
        message += std::string(argv[i]) + " ";
    }
}
void CollectorComponent::start() {
    logger->info("Starting collector component");
    logger->debug("to: " + to);
    logger->debug("selection: " + selection);
    logger->debug("performative: " + perStr);
    logger->debug("message: " + message);

    std::stringstream performativeConverter(perStr);
    Performative perf; 
    performativeConverter >> perf; 

    std::stringstream messageWriter; 
    messageWriter << selection 
        << " " << 500 
        << " " << perf 
        << " " << "collector"
        << " " << to 
        << " " << message;
    logger->debug("Sending message " + messageWriter.str());

    reply = sendReplyableMessage("activity_manager", Performative::REQUEST, 
        "send_message " + messageWriter.str()
    );
}

void CollectorComponent::stop() {
    logger->info("Collecting finalized");
}

void CollectorComponent::handleMessage(Message msg) {

    logger->debug("Got message: " + msg.body);
    if (msg.reply_with == reply && msg.from_component == "activity_manager") {
        std::stringstream parser(msg.body); 
        std::string host; 
        parser >> host; 
        if (host == "=") {
            parser >> size; 
        }
        else {
            size--; 
            std::string val; 
            std::getline(parser, val);
            std::cout << host << " |> " << val << std::endl;
        }
    }
    if (size <= 0) {
        finish();
    }
}
