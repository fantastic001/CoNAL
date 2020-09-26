#pragma once 

#include "Message.hpp"
#include <thread>
#include "Logger.hpp"

namespace conal {
    namespace framework {
        class ComponentManager; 
        class Component {

                friend class ComponentManager;

                std::thread messageReadingThread;
            
            protected:
                std::shared_ptr<Logger> logger;

            public: 
                virtual void start() =0;
                virtual void stop() =0;
                virtual void handleMessage(Message msg) =0;

        };
    }
}