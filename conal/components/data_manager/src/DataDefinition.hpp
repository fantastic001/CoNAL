#pragma once 

#include <string>
#include <vector>
#include <map>

namespace conal {
    namespace data_manager {
        class DataDefinition {
                std::string name; 
                std::vector<std::string> params; 
                std::map<std::string, std::string> optional_params; 
            public:
                DataDefinition(std::string specification);

                std::string getName() const; 
                std::vector<std::string> getParams() const; 
                std::map<std::string, std::string> getOptionalParams() const; 

        };
    }
}