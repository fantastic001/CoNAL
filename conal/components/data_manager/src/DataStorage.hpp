#pragma once 

#include <DataDefinition.hpp>
#include <Source.hpp>
#include <SourceManager.hpp>
#include <Logger.hpp>

#include <memory>
#include <utility>
#include <mutex>
#include <sstream>
#include <list>


namespace conal {
    namespace data_manager {
        class DataStorage {
                std::map<std::string, std::shared_ptr<Source>> storage; 
                std::shared_ptr<SourceManager> sourceManager; 
                std::mutex storage_mutex;
                std::shared_ptr<::conal::framework::Logger> logger; 


            public:
                DataStorage(std::shared_ptr<::conal::framework::Logger> logger);
                bool create(std::string id, DataDefinition dataDefinition); 
                std::string getString(std::string id);

                template<typename ValueType> 
                ValueType getValue(std::string id) {
                    ValueType value; 
                    std::string valueStr = getString(id);
                    std::stringstream ss(valueStr);
                    ss >> value;
                    return value; 
                }

                std::list<std::string> getAllDataInstances();

                std::shared_ptr<Source> getSource(std::string id);


        };
    }
}