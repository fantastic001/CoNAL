#pragma once 

namespace conal {
    namespace framework {
        enum class Performative {
            REQUEST,
            START,
            ACCEPT,
            CREATE,
            STOP,
            REJECT,
            DESTROY,
            DATA
        };
    } 
}