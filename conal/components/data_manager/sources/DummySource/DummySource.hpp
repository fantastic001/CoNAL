#pragma once 

#include <vector> 
#include <string>
#include <map>
#include <optional>
#include <Source.hpp>

class DummySource : public conal::data_manager::Source {
        bool finished; 
    public:

        conal::data_manager::ErrorValue init(std::vector<std::string> params, std::map<std::string, std::string> optional_params);
        std::string getOne();
        bool end();
        virtual ~DummySource() {
            
        }
};
