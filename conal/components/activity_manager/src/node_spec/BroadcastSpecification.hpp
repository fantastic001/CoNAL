#pragma once

#include <Connection.hpp>
#include <memory>
#include "Specification.hpp"

namespace conal {
    namespace activity_manager {
        namespace node_spec {
            class BroadcastSpecification : public Specification {
                public:
                    virtual bool valid(std::shared_ptr<::conal::framework::Connection> connection)
                    {
                        return true;
                    }
            };
        }
    }
}