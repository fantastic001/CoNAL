#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <vector> 
#include <string>
#include <future>

namespace conal {
    namespace collector {
        class CollectorComponent : public conal::framework::Component {
                std::string selection;
                int reply; 
                int size; 
                std::string perStr; 
                std::string to; 
                std::string message; 
                

            public: 
                explicit CollectorComponent(int argc, char** argv);
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);
        };
    }
}