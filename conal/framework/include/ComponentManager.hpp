#pragma once 

#include "Component.hpp"
#include <memory>

namespace conal {
    namespace framework {
        class ComponentManager {
            public:
                void registerComponent(std::string name, std::shared_ptr<Component> component);
                static std::shared_ptr<ComponentManager> getInstance();
                ComponentManager(ComponentManager const&) = delete;
                void operator=(ComponentManager const&) = delete;
            private:
                explicit ComponentManager();
                static std::shared_ptr<ComponentManager> instance; 
                


        };
    }
} 