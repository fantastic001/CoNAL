#include <DummyLoader.hpp>

#include <thread>
#include <iostream>

using namespace conal::code_manager;
using namespace std; 

bool DummyLoader::validate(std::string path, std::vector<std::string> params, EnvParams env) {
    return path == "TEST"; 
}
std::string DummyLoader::load(std::string path, std::vector<std::string> params, EnvParams env) {
    return "XXX";
}

void DummyLoader::run(std::string code, std::vector<DataBinding> in, std::vector<DataBinding> out) {
    thread process([in, out] () {
        this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Hello world\n";
        for (auto input : in) {
            if (input.getIdentifier() == "test") {
                std::cout << (input.end() ? "finalized\n" : input.get() + "\n");
            }
        }
    });
    process.detach();

    
}

extern "C" ::conal::code_manager::Loader* createInstance() {
    return new DummyLoader();
}
