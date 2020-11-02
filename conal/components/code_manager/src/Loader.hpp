#pragma once 

#include <vector> 
#include <string>

namespace conal {
    namespace code_manager {
        class Loader {
            public:
                virtual bool validate(std::string path, std::vector<std::string> params)=0;
                virtual std::string load(std::string path, std::vector<std::string> params)=0;
                virtual ~Loader() {
                    
                }
        };
    }
}