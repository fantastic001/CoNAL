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
        std::pair<std::shared_ptr<Source>, std::shared_ptr<Source>> split();
        
        std::string serialize();

        conal::data_manager::ErrorValue deserialize(std::string code);

        std::string dump(); 

        virtual ~DummySource() {
            
        }
};
