#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <vector> 
#include <string>

namespace conal {
    namespace code_manager {
        class CodeManagerComponent : public conal::framework::Component {
                
            public: 
                explicit CodeManagerComponent();
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);
        };
    }
}