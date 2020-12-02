
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

Message Component::sendMessageAndWait(std::string to_component, Performative performative, std::string body) {
        auto os = std::fstream(
            std::string(std::getenv("COMPONENT_COMM_DIR")) + "/" + to_component + "/messages", 
            std::ios::out
        );
        Message mess(performative, name, to_component, body);
        mess.reply_with = 1+std::abs((int) rand());
        logger->debug("Locking context");
        std::unique_lock<std::mutex> lock(m);
        logger->debug("Inserting CV into map");
        waitingForReply.emplace(std::piecewise_construct, std::make_tuple(mess.reply_with), std::make_tuple());
        notServed[mess.reply_with] = std::make_pair(false, Message());
        logger->debug("Sending message to another component");
        os << mess;
        os.flush();
        lock.unlock();
        logger->debug("Waiting for reply....");
        while (! notServed[mess.reply_with].first) 
        {
            logger->debug("Starting to wait");
            waitingForReply[mess.reply_with].wait(lock);
        }
        logger->debug("Got waited message");
        auto res = notServed[mess.reply_with].second;
        logger->debug(res.body);
        waitingForReply.erase(mess.reply_with);
        notServed.erase(mess.reply_with);
        return res;
}

void Component::deliver(Message msg) {
    if (msg.reply_with == 0) {
        handleMessage(msg);
    }
    else {
        std::unique_lock<std::mutex> lock(m);
        if (waitingForReply.find(msg.reply_with) != waitingForReply.end()) {
            notServed[msg.reply_with] = std::make_pair(true, msg);
            lock.unlock();
            waitingForReply[msg.reply_with].notify_all();
            return;
        }
        else {
            handleMessage(msg);
        }
    }
}

void Component::reply(Message msg, Performative performative, std::string body) {
    Message mess(performative, this->name, msg.from_component, body);
    mess.reply_with = msg.reply_with;
    auto os = std::fstream(
            std::string(std::getenv("COMPONENT_COMM_DIR")) + "/" + msg.from_component + "/messages", 
            std::ios::out
        );
    os << mess;
}
