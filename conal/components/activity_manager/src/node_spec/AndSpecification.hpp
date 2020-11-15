#pragma once

#include <Connection.hpp>
#include <memory>
#include <node_spec/Specification.hpp>

namespace conal {
    namespace activity_manager {
        namespace node_spec {
            class AndSpecification : public Specification{
                    Specification* spec1, *spec2; 
                public:

                    AndSpecification(Specification* spec1, Specification* spec2) {
                        this->spec1 = spec1;
                        this->spec2 = spec2;
                    }
                    virtual bool valid(std::shared_ptr<::conal::framework::Connection> connection)
                    {
                        return true; 
                    }

                    ~AndSpecification() {
                        delete spec1; 
                        delete spec2;
                    }
            };
        }
    }
}