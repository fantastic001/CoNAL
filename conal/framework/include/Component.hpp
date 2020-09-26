#pragma once 

#include "Message.hpp"

namespace conal {
    namespace framework {
        class Component {

            public: 
                virtual void start() =0;
                virtual void stop() =0;
                virtual void handleMessage(Message msg) =0;

        };
    }
}