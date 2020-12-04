
#pragma once 

#include <string>
#include "Performative.hpp"
#include <iostream>

namespace conal {
    namespace framework {
        struct Message {
            ::conal::framework::Performative performative;
            std::string from_component;
            std::string to_component;
            std::string body;
            int reply_with = 0;

            Message(
                ::conal::framework::Performative performative,
                std::string from_component,
                std::string to_component,
                std::string body) {
                    this->performative = performative; 
                    this->from_component = from_component;
                    this->to_component = to_component;
                    this->body = body;
            }
            Message() {
                
            }
        };
        std::ostream& operator<<(std::ostream& out, const conal::framework::Message& message);
        std::istream& operator>>(std::istream& ss,conal::framework::Message& message);
    }
}
