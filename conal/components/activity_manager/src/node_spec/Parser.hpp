#pragma once 

#include <node_spec/Specification.hpp>

namespace conal {
    namespace activity_manager {
        namespace node_spec {
            class Parser {
                public:
                    Specification* parse(const char* code); 
            };
        }
    }
}