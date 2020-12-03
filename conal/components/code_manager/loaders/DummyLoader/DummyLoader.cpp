#include <DummyLoader.hpp>

#include <iostream>

using namespace conal::code_manager;

bool DummyLoader::validate(std::string path, std::vector<std::string> params, EnvParams env) {
    return path == "TEST"; 
}
std::string DummyLoader::load(std::string path, std::vector<std::string> params, EnvParams env) {
    return "XXX";
}

void DummyLoader::run(std::string code, std::vector<DataBinding> in, std::vector<DataBinding> out) {
    std::cout << "Hello world\n";
}

extern "C" ::conal::code_manager::Loader* createInstance() {
    return new DummyLoader();
}
