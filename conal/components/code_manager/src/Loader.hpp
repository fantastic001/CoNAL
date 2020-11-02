#pragma once 

#include <vector> 
#include <string>

namespace conal {
    namespace code_manager {
        class Loader {
            public:
                virtual std::string load(std::string path, std::vector<std::string> params)=0;
        };
    }
}