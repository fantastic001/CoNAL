#pragma once 

#include <Loader.hpp>

class CppLoader : public ::conal::code_manager::Loader {
    bool validate(std::string path, std::vector<std::string> params, EnvParams env);
    std::string load(std::string path, std::vector<std::string> params, EnvParams env);
    void run(std::string code, std::vector<conal::code_manager::DataBinding> in, std::vector<conal::code_manager::DataBinding> out);

    std::string findToolChain(EnvParams env);

    std::string encode(std::string a);
    std::string decode(std::string a);
};