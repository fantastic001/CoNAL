#pragma once 

#include <vector> 
#include <string>
#include <map>
#include <optional>
#include <Source.hpp>

class Variable : public conal::data_manager::Source {
        std::string value;
    public:

        conal::data_manager::SourceCopy init(std::vector<std::string> params, std::map<std::string, std::string> optional_params);
        std::string getOne();
        bool end();
        std::pair<std::shared_ptr<Source>, std::shared_ptr<Source>> split();
        
        std::string serialize();

        conal::data_manager::SourceCopy deserialize(std::string code);

        std::string dump(); 

        bool add(std::string);
        std::optional<std::string> at(std::string);

        virtual ~Variable() {
            
        }
};
