
#include <DataManagerComponent.hpp>
#include <iostream>
#include <sstream>

using namespace conal::data_manager; 
using namespace conal::framework;

DataManagerComponent::DataManagerComponent() {
}
void DataManagerComponent::start() {
    logger->info("Starting Data manager component");
    sourceManager = std::make_shared<SourceManager>(logger);
}

void DataManagerComponent::stop() {

}

void DataManagerComponent::handleMessage(Message msg) {
    logger->debug("Got message: " + msg.body);
}

