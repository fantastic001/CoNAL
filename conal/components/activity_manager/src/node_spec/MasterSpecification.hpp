#pragma once

#include <Connection.hpp>
#include <memory>
// #include "Specification.hpp"
#include <node_spec/Specification.hpp>

namespace conal {
    namespace activity_manager {
        namespace node_spec {
            class MasterSpecification : public Specification {
                public:
                    virtual bool valid(std::shared_ptr<::conal::framework::Connection> connection)
                    {
                        return true;
                    }

                    std::string dump() {
                        return "%";
                    }
            };
        }
    }
}