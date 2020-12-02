
#include <DataManagerComponent.hpp>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace conal::data_manager; 
using namespace conal::framework;
using namespace std; 

DataManagerComponent::DataManagerComponent() {
}
void DataManagerComponent::start() {
    logger->info("Starting Data manager component");
    storage = std::make_shared<DataStorage>(logger);
}

void DataManagerComponent::stop() {

}

void DataManagerComponent::handleMessage(Message msg) {
    logger->debug("Got message: " + msg.body);
    if (msg.performative == Performative::CREATE) {
        // Format <DataIdentifier>=<Specification>@<Selection>
        auto equalsSepPos = msg.body.find_first_of('=');
        auto hashSepPos = msg.body.find_first_of('@');
        if (hashSepPos - equalsSepPos < 1) {
            logger->error("Wrong format for message to data manager.");
            return;
        }
        string id = msg.body.substr(0, equalsSepPos);
        string specification = msg.body.substr(equalsSepPos+1, hashSepPos-equalsSepPos);
        string selection;
        if (hashSepPos >= msg.body.size() -1) {
            logger->debug("This data instance won't be distributed and split");
            selection = "";
        }
        else selection = msg.body.substr(hashSepPos+1);
        dataCreationthread = std::thread(&DataManagerComponent::createDataInstance, this, id, specification, selection);
        dataCreationthread.detach();
    }

}

void DataManagerComponent::createDataInstance(std::string id, std::string specification, std::string selection)
{
        if(! storage->create(id, DataDefinition(specification))) {
            logger->error("Couldn't create data instance");
            return;
        }
        logger->debug("Created data instance: " + id);
        if (! selection.empty()) {
            logger->debug("Selection is not empty, so we are finding out hostnames from AM");
            auto res = sendMessageAndWait("activity_manager", Performative::REQUEST, "list " + selection);
            logger->debug("Got message: " + res.body);
            splitMessages(res, id);
            logger->debug("Finalized delivery to clients");
        }
}


void DataManagerComponent::splitMessages(::conal::framework::Message msg, std::string id)
{
    stringstream reader(msg.body);
    std::vector<std::string> hostnames;
    while (reader.good()) {
        string hostname; 
        reader >> hostname;
        if (! hostname.empty()) hostnames.push_back(hostname);
    }
    auto s = (storage->getSource(id));
    logger->debug("Splitting data");
    auto result = split(hostnames.size(), s);
    int i = 0; 
    logger->debug("Sending data to clients");
    for (i=0; i<hostnames.size(); i++) {
        string code = result[i]->serialize();
        logger->debug("Code: " + code);
        sendMessage("activity_manager", Performative::REQUEST, "send name=" + hostnames[i] + "#DATA " + id + "=" + code + "@");
    }
}

vector<shared_ptr<Source>> DataManagerComponent::split(int size, shared_ptr<Source> source) {
    vector<shared_ptr<Source>> result; 
    if (size == 0) return result; 
    else if (size == 1) {
        result.push_back(source);
    }
    else if (size == 2) {
        auto p = source->split();
        result.push_back(p.first);
        result.push_back(p.second);
    }
    else {
        auto p = source->split();
        int half = size / 2; 
        auto first = split(half, p.first);
        auto second = split(size - half, p.second);
        std::merge(first.begin(), first.end(), second.begin(), second.end(), result.begin());
    }
    return result;
}