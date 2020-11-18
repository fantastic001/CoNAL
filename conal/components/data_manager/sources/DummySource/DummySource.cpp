
#include <DummySource.hpp>

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

extern "C" Source* createInstance() {
    return new DummySource();
}