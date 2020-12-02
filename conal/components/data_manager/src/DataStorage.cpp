
#include <DataStorage.hpp>

using namespace conal::data_manager; 
using namespace std; 
using Logger = ::conal::framework::Logger;

DataStorage::DataStorage(std::shared_ptr<Logger> logger) : logger(logger) {
    sourceManager = make_shared<SourceManager>(logger);
    logger->info("Initialized storage for data");
}

bool DataStorage::create(std::string id, DataDefinition dataDefinition) {
    unique_lock<mutex> lock(storage_mutex);
    if (storage.find(id) != storage.end()) {
        logger->error("Data source with identifier " + id + " already existing");
        return false;
    }
    string name = dataDefinition.getName();
    auto params = dataDefinition.getParams();
    auto optionalParams = dataDefinition.getOptionalParams();

    auto name_source_pair = sourceManager->findSource(name, params, optionalParams);
    if (name_source_pair.second) {
        logger->debug("SourceManager created data instance");
        storage[id] = (name_source_pair).second;
        return true;
    }
    else return false;
}

string DataStorage::getString(string id) {
    unique_lock<mutex> lock(storage_mutex);
    storage[id]->getOne();
}

std::shared_ptr<Source> DataStorage::getSource(string id) {
    return storage[id];
}