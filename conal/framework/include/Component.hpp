#pragma once 

#include "Message.hpp"
#include <thread>
#include "Logger.hpp"
#include <condition_variable>
#include <random>
#include <map>
#include <mutex>

namespace conal {
    namespace framework {
        class ComponentManager;
        
        /*! Represents specific component in CoNAL environment.
        */
        class Component {

                friend class ComponentManager;

                std::thread messageReadingThread;
                std::default_random_engine rand;
                std::string name;
                std::map<int, std::condition_variable> waitingForReply;
                std::map<int, std::pair<bool, Message>> notServed;
                std::mutex m; 
            
            protected:
                std::shared_ptr<Logger> logger;
                void sendMessage(std::string to_component, Performative performative, std::string body);
                void reply(Message msg, Performative performative, std::string body);
                Message sendMessageAndWait(std::string to_component, Performative performative, std::string body);
                Component();
            public: 
                // needs to be implemented by component. 
                // This method is called when component is started.
                virtual void start() =0;

                // needs to be implemented by component. 
                // This method is called when component is stopped.
                virtual void stop() =0;

                // needs to be implemented by component. This method is called 
                // when component gets internal message from other 
                // component or from user through environment.
                virtual void handleMessage(Message msg) =0;

                void deliver(Message msg);

        };
    }
}