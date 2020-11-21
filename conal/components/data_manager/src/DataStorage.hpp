#pragma once 

#include <DataDefinition.hpp>
#include <Source.hpp>
#include <SourceManager.hpp>
#include <Logger.hpp>

#include <memory>
#include <utility>
#include <mutex>
#include <sstream>


namespace conal {
    namespace data_manager {
        class DataStorage {
                std::vector<std::pair<std::string, std::shared_ptr<Source>>> storage; 
                std::shared_ptr<SourceManager> sourceManager; 
                std::mutex storage_mutex;
                std::shared_ptr<::conal::framework::Logger> logger; 
            public:
                DataStorage(std::shared_ptr<::conal::framework::Logger> logger);
                int create(DataDefinition dataDefinition); 

                std::string getString(int id);
                
                template<typename ValueType> 
                ValueType getValue(int id) {
                    ValueType value; 
                    std::string valueStr = getString(id);
                    std::stringstream ss(valueStr);
                    ss >> value;
                    return value; 
                }

                std::string getSourceName(int id);
                std::shared_ptr<Source> getSource(int id);


        };
    }
}