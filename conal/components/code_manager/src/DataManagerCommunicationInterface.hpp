#pragma once 

#include <string>

namespace conal {
    namespace code_manager {
        class DataManagerCommunicationInterface {
                /*
                All functions in this implementation should block until request is not
                fullfilled 
                */
            public:


                virtual std::string sendGetRequest(std::string identifier) =0; 
                virtual std::string sendAtRequest(std::string identifier, std::string key) =0; 
                virtual bool sendEndRequest(std::string identifier) =0; 
                virtual bool sendAddRequest(std::string identifier, std::string data) =0; 
        };
    }
}