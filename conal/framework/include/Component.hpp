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
            
            protected:
                std::shared_ptr<Logger> logger;
                void sendMessage(std::string to_component, Performative performative, std::string body);
                void reply(Message msg, std::string body, Performative performative = Performative::REPLY);
                int sendReplyableMessage(std::string to_component, Performative performative, std::string body);
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