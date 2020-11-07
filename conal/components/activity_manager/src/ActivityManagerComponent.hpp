#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <TCPServer.hpp>
#include <TCPCLient.hpp>
#include "ConnectionManager.hpp"
#include <Task.hpp>
namespace conal {
    namespace activity_manager {
        class ActivityManagerComponent : public conal::framework::Component {
                std::shared_ptr<::conal::framework::TCPServer> server; 
                std::thread serverThread;
                ConnectionManager connectionManager;
                std::string masterHostname;
                std::shared_ptr<::conal::framework::TCPClient> clientPtr;
                std::vector<Task> tasks; 
                std::mutex task_mutex;

                void runServer(); 
            public: 
                explicit ActivityManagerComponent();
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);
                bool isSlave() const;
        };
    }
}