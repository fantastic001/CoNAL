#include <DummyLoader.hpp>

#include <iostream>

bool DummyLoader::validate(std::string path, std::vector<std::string> params, EnvParams env) {
    return true; 
}
std::string DummyLoader::load(std::string path, std::vector<std::string> params, EnvParams env) {
    return "XXX";
}

void DummyLoader::run(std::string code) {
    std::cout << "Hello world\n";
}

extern "C" ::conal::code_manager::Loader* createInstance() {
    return new DummyLoader();
}
