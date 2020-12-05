#pragma once 

#include <string>
#include <DataManagerCommunicationInterface.hpp>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <list>

#include <DataChangeListener.hpp>
#include <memory>

namespace conal {
    namespace code_manager {
        class CodeManagerComponent; 
        class DataBinding {
            friend class CodeManagerComponent; 
            std::string identifier; 
            DataManagerCommunicationInterface* comm; 
            std::list<std::shared_ptr<DataChangeListener>> listeners;

            DataBinding() = delete;
            DataBinding(std::string identifier, DataManagerCommunicationInterface* comm) {
                this->identifier = identifier;
                this->comm = comm;
            }
            void notify() {
                for (auto& listener : listeners) {
                    listener->onDataChange(*this);
                }
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

            void listen(std::shared_ptr<DataChangeListener> listener) {
                listeners.push_back(listener);
            }

           
        };

        using DataBindings = std::vector<DataBinding>;
    }
}