#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <TCPServer.hpp>

#include "ConnectionManager.hpp"

namespace conal {
    namespace activity_manager {
        class ActivityManagerComponent : public conal::framework::Component {
                std::shared_ptr<::conal::framework::TCPServer> server; 
                std::thread serverThread;
                ConnectionManager connectionManager;

                void runServer(); 
            public: 
                explicit ActivityManagerComponent();
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);
        };
    }
}