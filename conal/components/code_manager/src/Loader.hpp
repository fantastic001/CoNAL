#pragma once 

#include <vector> 
#include <string>
#include <map>

using EnvParams = std::map<std::string, std::string>;

namespace conal {
    namespace code_manager {
        class Loader {
            public:
                // Should return if particular loader can load code with given path, params and 
                // information of environment parameters got from environment 
                virtual bool validate(std::string path, std::vector<std::string> params, EnvParams env)=0;

                // this function should return serialized version of loadable program which can be used to 
                // run and transfer code across platforms 
                virtual std::string load(std::string path, std::vector<std::string> params, EnvParams env)=0;

                // this is implementation of running serialized code got from load(...) method.
                virtual void run(std::string code) =0;
                virtual ~Loader() {
                    
                }
        };
    }
}