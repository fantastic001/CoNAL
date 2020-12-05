
#include <DummySource.hpp>
#include <utility>

using namespace conal::data_manager;

SourceCopy DummySource::init(std::vector<std::string> params, std::map<std::string, std::string> optional_params) {
    auto instance = std::make_shared<DummySource>();
    instance->finished = false; 
    finished = false;
    return SourceCopy(instance);
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
    return "XXX";
}

std::string DummySource::dump() {
    return "DummySource()";
}

conal::data_manager::SourceCopy DummySource::deserialize(std::string code) {
    return SourceCopy(std::make_shared<DummySource>());
}


bool DummySource::add(std::string data) {
    return true; 
}
std::optional<std::string> DummySource::at(std::string key) {
    return std::optional("TEST");
}


extern "C" Source* createInstance() {
    return new DummySource();
}