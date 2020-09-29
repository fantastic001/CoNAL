#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <TCPServer.hpp>

namespace conal {
    namespace activity_manager {
        class ActivityManagerComponent : public conal::framework::Component {
                std::shared_ptr<::conal::framework::TCPServer> server; 
            public: 
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);
        };
    }
}