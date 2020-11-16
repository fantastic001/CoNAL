#pragma once

#include <Connection.hpp>
#include <memory>
// #include "Specification.hpp"
#include <node_spec/Specification.hpp>

namespace conal {
    namespace activity_manager {
        namespace node_spec {
            class ParamSpecification : public Specification {
                    std::string param, value; 
                public:
                    ParamSpecification(std::string param, std::string value) 
                        : param(param), value(value) {

                        }
                    virtual bool valid(std::shared_ptr<::conal::framework::Connection> connection)
                    {
                        return connection->getProperty(param) == value;
                    }

                    std::string dump() {
                        return "[" + param + "]=" + value;
                    }
            };
        }
    }
}