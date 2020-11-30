
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
        stringstream bodyReader(msg.body);
        string specification; 
        std::getline(bodyReader, specification);
        stringstream reply; 
        int id = storage->create(DataDefinition(specification));
        reply << id << " " << storage->getSourceName(id) << " " << specification;
        sendMessage(msg.from_component, Performative::DATA, reply.str()); 
    }
    else if (msg.performative == Performative::REQUEST) {
        stringstream reader(msg.body);
        string hostname;
        int id; 
        reader >> id;
        reader >> hostname;
        if (idsToHostnamesMapping.find(id) != idsToHostnamesMapping.end()) {
            idsToHostnamesMapping[id].push_back(hostname);
        }
        else {
            idsToHostnamesMapping[id] = list<string>();
            idsToHostnamesMapping[id].push_back(hostname);
        }
    }
    else if (msg.performative == Performative::START) {
        stringstream reader(msg.body);
        int id; 
        reader >> id; 
        auto hostnames = idsToHostnamesMapping[id];
        auto s = (storage->getSource(id));
        auto result = split(hostnames.size(), s);
        int i = 0; 
        for (auto hostname : hostnames) {
            string code = result[i]->serialize();
            string name = storage->getSourceName(id);
            stringstream writer; 
            writer << hostname << " " << id << " " << name << " " << code;
            sendMessage(msg.from_component, Performative::START, writer.str());
        }
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