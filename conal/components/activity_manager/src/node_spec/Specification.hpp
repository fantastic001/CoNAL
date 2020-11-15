#pragma once

#include <Connection.hpp>
#include <memory>

namespace conal {
    namespace activity_manager {
        namespace node_spec {
            class Specification {
                public:
                    virtual bool valid(std::shared_ptr<::conal::framework::Connection> connection) =0;
                    virtual ~Specification();
            };
        }
    }
}