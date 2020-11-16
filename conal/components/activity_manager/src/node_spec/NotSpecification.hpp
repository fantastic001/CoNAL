#pragma once

#include <Connection.hpp>
#include <memory>
// #include "Specification.hpp"
#include <node_spec/Specification.hpp>

namespace conal {
    namespace activity_manager {
        namespace node_spec {
            class NotSpecification : public Specification {
                    Specification* spec; 
                public:
                    NotSpecification(Specification* spec) : spec(spec) {

                    }
                    virtual bool valid(std::shared_ptr<::conal::framework::Connection> connection)
                    {
                        return ! spec->valid(connection);
                    }

                    std::string dump() {
                        return "!" + spec->dump();
                    }
            };
        }
    }
}