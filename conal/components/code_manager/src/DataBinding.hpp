#pragma once 

#include <string>
#include <DataManagerCommunicationInterface.hpp>
#include <memory>

namespace conal {
    namespace code_manager {
        class CodeManagerComponent; 
        class DataBinding {
            friend class CodeManagerComponent; 
            std::string identifier; 
            DataManagerCommunicationInterface* comm; 
            DataBinding(std::string identifier, DataManagerCommunicationInterface* comm) {
                this->identifier = identifier;
                this->comm = comm;

            } 
        public:

            std::string getIdentifier() const {
                return identifier; 
            }

            std::string get() {
                return comm->sendGetRequest(identifier);
            }

            std::string at(std::string key) {
                return comm->sendAtRequest(identifier, key);
            }

            bool end() {
                return comm->sendEndRequest(identifier); 
            }

            bool add(std::string data) {
                return comm->sendAddRequest(identifier, data);
            }
        };

        using DataBindings = std::vector<DataBinding>;
    }
}