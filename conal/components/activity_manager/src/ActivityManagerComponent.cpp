
#include <ActivityManagerComponent.hpp>
#include <iostream>


using namespace conal::activity_manager; 
using namespace conal::framework;

void ActivityManagerComponent::start() {
    std::cout << "Starting activity manager\n";
}

void ActivityManagerComponent::stop() {

}

void ActivityManagerComponent::handleMessage(Message msg) {
    std::cout << "Got message " << msg.body << std::endl;
}