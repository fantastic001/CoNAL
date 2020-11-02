#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <vector> 
#include <string>
#include <LoaderManager.hpp> 

namespace conal {
    namespace code_manager {
        class CodeManagerComponent : public conal::framework::Component {
            std::shared_ptr<::conal::code_manager::LoaderManager> loaderManager;
                
            public: 
                explicit CodeManagerComponent();
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);
        };
    }
}