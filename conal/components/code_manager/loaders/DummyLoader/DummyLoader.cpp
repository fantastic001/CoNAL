#include <DummyLoader.hpp>



bool DummyLoader::validate(std::string path, std::vector<std::string> params) {
    return false; 
}
std::string DummyLoader::load(std::string path, std::vector<std::string> params) {
    return "";
}

void DummyLoader::run(std::string code) {
    
}

extern "C" ::conal::code_manager::Loader* createInstance() {
    return new DummyLoader();
}
