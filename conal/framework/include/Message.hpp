
#pragma once 

#include <string>
#include "Performative.hpp"

namespace conal {
    namespace framework {
        struct Message {
            ::conal::framework::Performative performative;
            std::string from_component;
            std::string to_component;
            std::string body;
        };
    }
}