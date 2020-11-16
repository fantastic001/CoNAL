#pragma once

#include <Connection.hpp>
#include <memory>
#include <node_spec/Specification.hpp>

namespace conal {
    namespace activity_manager {
        namespace node_spec {
            class OrSpecification : public Specification{
                    Specification *lhs, *rhs; 
                public:
                    OrSpecification(Specification *lhs, Specification* rhs) :
                        lhs(lhs), rhs(rhs) {

                        }
                    virtual bool valid(std::shared_ptr<::conal::framework::Connection> connection)
                    {
                        return lhs->valid(connection) || rhs->valid(connection);
                    }

                    std::string dump() {
                        // return spec1->dump() + "+" + spec2->dump();
                        return lhs->dump() + "+" + rhs->dump();
                    }
                    ~OrSpecification() {
                        delete lhs;
                        delete rhs;
                    }
            };
        }
    }
}