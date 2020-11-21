
#include <DataStorage.hpp>

using namespace conal::data_manager; 
using namespace std; 
using Logger = ::conal::framework::Logger;

DataStorage::DataStorage(std::shared_ptr<Logger> logger) : logger(logger) {
    sourceManager = make_shared<SourceManager>(logger);
    logger->info("Initialized storage for data");
}

int DataStorage::create(DataDefinition dataDefinition) {
    unique_lock<mutex> lock(storage_mutex);
    string name = dataDefinition.getName();
    auto params = dataDefinition.getParams();
    auto optionalParams = dataDefinition.getOptionalParams();

    auto name_source_pair = sourceManager->findSource(name, params, optionalParams);
    if (name_source_pair.second) storage.push_back(name_source_pair);
}

string DataStorage::getString(int id) {
    unique_lock<mutex> lock(storage_mutex);
    storage[id].second->getOne();
}

string DataStorage::getSourceName(int id) {
    return storage[id].first; 
}


std::shared_ptr<Source> DataStorage::getSource(int id) {
    return storage[id].second; 
}