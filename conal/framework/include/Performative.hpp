#pragma once 
#include <iostream>

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
            DATA,
            REPLY,
            NOTIFY
        };

        std::istream& operator>> (std::istream& in, Performative& perf);
        std::ostream& operator<< (std::ostream& in, const Performative& perf);

        std::string performative_to_string(Performative p);
    } 
}