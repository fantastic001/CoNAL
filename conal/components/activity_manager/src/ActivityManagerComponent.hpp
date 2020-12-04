#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <TCPServer.hpp>
#include <TCPCLient.hpp>
#include "ConnectionManager.hpp"
#include <Task.hpp>
#include <future>
namespace conal {
    namespace activity_manager {
        class ActivityManagerComponent : public conal::framework::Component {
                std::shared_ptr<::conal::framework::TCPServer> server; 
                std::thread serverThread;
                std::thread clientThread;

                ConnectionManager connectionManager;
                std::string masterHostname;
                std::shared_ptr<::conal::framework::TCPClient> clientPtr;
                std::vector<Task> tasks; 
                std::mutex task_mutex;

                std::map<int, std::string> clientTaskIdToCodeMapping;
                std::map<int, std::string> clientTaskIdToLoaderMapping;
                std::map<int, std::vector<std::string>> clientTaskIdToParamsMapping;

                using ConnectionList = std::list<std::shared_ptr<conal::framework::Connection>>;
                std::map<std::string, ConnectionList> dataSpecToConnectionsMapping; 
                std::map<std::string, int> dataSpecToIdMapping;
                std::map<int, std::string> dataIdToSourceNameMapping;  

                std::map<int, conal::framework::Message> waitingFromClients;
                std::map<int, conal::framework::Message> remoteMessageReplies; 
                std::map<int, std::promise<conal::framework::Message>> waitingFromComponent;

                void runServer(); 
                void runClient();
            public: 
                explicit ActivityManagerComponent();
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);
                void handleMessageFromCodeManager(::conal::framework::Message msg); 
                void handleMessageFromDataManager(::conal::framework::Message msg); 
                void handleMessageFromUser(::conal::framework::Message msg); 
                bool isSlave() const;
                void handleClientReply(std::shared_ptr<::conal::framework::TCPClient> conn, std::string reply);
                void sendFromClient(std::string hostname, conal::framework::Message msg);

                std::string clearDataSpecification(std::string spec);
        };
    }
}