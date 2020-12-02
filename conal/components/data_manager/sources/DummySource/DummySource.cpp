
#include <DummySource.hpp>
#include <utility>

using namespace conal::data_manager;

ErrorValue DummySource::init(std::vector<std::string> params, std::map<std::string, std::string> optional_params) {
    finished = false;
    return ErrorValue();
}


std::string DummySource::getOne() {
    finished = true;
    return "TEST";
}


bool DummySource::end() {
    return finished;
}

std::pair<std::shared_ptr<Source>, std::shared_ptr<Source>> DummySource::split() {
    auto first = std::make_shared<DummySource>();
    auto second = std::make_shared<DummySource>();
    first->init(std::vector<std::string>(), std::map<std::string, std::string>());
    second->init(std::vector<std::string>(), std::map<std::string, std::string>());
    return std::make_pair(first, second); 
}


std::string DummySource::serialize() {
    return "DummySource()";
}

conal::data_manager::ErrorValue DummySource::deserialize(std::string code) {
    return ErrorValue();
}



extern "C" Source* createInstance() {
    return new DummySource();
}