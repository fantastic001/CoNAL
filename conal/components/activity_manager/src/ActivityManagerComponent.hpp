#pragma once 

#include <Component.hpp>
#include <Message.hpp>

namespace conal {
    namespace activity_manager {
        class ActivityManagerComponent : public conal::framework::Component {
            public: 
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);
        };
    }
}