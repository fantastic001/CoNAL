#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <vector> 
#include <string>
#include <SourceManager.hpp> 

namespace conal {
    namespace data_manager {
        class DataManagerComponent : public conal::framework::Component {
                using SourcePtr = std::pair<std::string, std::shared_ptr<Source>>;
                using LoadingResult = std::pair<std::string, SourcePtr>;
                std::shared_ptr<::conal::data_manager::SourceManager> sourceManager;
                
                std::map<std::pair<int, std::string>, LoadingResult> taskIdHostnameToDataMapping;

            public: 
                explicit DataManagerComponent();
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);
        };
    }
}