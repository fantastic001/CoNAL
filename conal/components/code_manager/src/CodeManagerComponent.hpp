#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <vector> 
#include <string>
#include <LoaderManager.hpp> 

namespace conal {
    namespace code_manager {
        class CodeManagerComponent : public conal::framework::Component {
                using LoaderPtr = std::pair<std::string, std::shared_ptr<Loader>>;
                using LoadingResult = std::pair<std::string, LoaderPtr>;
                std::shared_ptr<::conal::code_manager::LoaderManager> loaderManager;
                
                std::map<std::pair<int, std::string>, LoadingResult> taskIdHostnameToCodeMapping;
                EnvParams getEnvParams();

            public: 
                explicit CodeManagerComponent();
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);
        };
    }
}