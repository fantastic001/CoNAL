
#include <Loader.hpp>

class DummyLoader : public ::conal::code_manager::Loader {
    bool validate(std::string path, std::vector<std::string> params);
    std::string load(std::string path, std::vector<std::string> params);
};