#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <vector> 
#include <string>
#include <list>
#include <DataStorage.hpp>

namespace conal {
    namespace data_manager {
        class DataManagerComponent : public conal::framework::Component {
                std::shared_ptr<DataStorage> storage;
                std::map<int, std::string> msgReplyToIdMapping;
                

                std::vector<std::shared_ptr<Source>> split(int size, std::shared_ptr<Source> source);
                void splitMessages(::conal::framework::Message msg, std::string id);

            public: 
                explicit DataManagerComponent();
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);

                void createDataInstance(std::string id, std::string specification, std::string selection);

        };
    }
}