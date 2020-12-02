#pragma once

#include <Connection.hpp>
#include <memory>
// #include "Specification.hpp"
#include <node_spec/Specification.hpp>


namespace conal {
    namespace activity_manager {
        namespace node_spec {
            class NameSpecification : public Specification{
                    std::string name; 
                public:
                    NameSpecification(std::string name) : name(name) {

                    }
                    bool valid(std::shared_ptr<::conal::framework::Connection> connection)
                    {
                        return connection->getHostname() == name;
                    }

                    std::string dump() {
                        return "name=" + name;
                    }
            };
        }
    }
}