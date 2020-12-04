
#include <Component.hpp>
#include <fstream>
#include <Message.hpp>
#include <random>
#include <chrono>
#include <utility>


using namespace ::conal::framework;
Component::Component() {
    typedef std::chrono::high_resolution_clock myclock;
    auto seed = myclock::now().time_since_epoch().count();
    rand.seed(seed);
}

void Component::sendMessage(std::string to_component, Performative performative, std::string body) {
        auto os = std::fstream(
            std::string(std::getenv("COMPONENT_COMM_DIR")) + "/" + to_component + "/messages", 
            std::ios::out
        );
        os << Message(performative, name, to_component, body);
}

int Component::sendReplyableMessage(std::string to_component, Performative performative, std::string body) {
        auto os = std::fstream(
            std::string(std::getenv("COMPONENT_COMM_DIR")) + "/" + to_component + "/messages", 
            std::ios::out
        );
        Message mess(performative, name, to_component, body);
        mess.reply_with = 1+std::abs((int) rand());
        os << mess;
        return mess.reply_with;
}

void Component::deliver(Message msg) {
    handleMessage(msg);
}

void Component::reply(Message msg, std::string body, Performative performative) {
    logger->debug("Sending reply to " + msg.from_component);
    Message mess(performative, this->name, msg.from_component, body);
    mess.reply_with = msg.reply_with;
    auto os = std::fstream(
            std::string(std::getenv("COMPONENT_COMM_DIR")) + "/" + msg.from_component + "/messages", 
            std::ios::out
        );
    os << mess;
}
