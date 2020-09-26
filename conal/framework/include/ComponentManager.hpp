#pragma once 

#include "Component.hpp"
#include <memory>
#include "Logger.hpp"

namespace conal {
    namespace framework {
        class Logger;
        class ComponentManager {
            public:
                void registerComponent(std::string name, std::shared_ptr<Component> component);
                static std::shared_ptr<ComponentManager> getInstance();
                ComponentManager(ComponentManager const&) = delete;
                void operator=(ComponentManager const&) = delete;
            private:
                explicit ComponentManager();
                static std::shared_ptr<ComponentManager> instance; 
                Logger logger;
                


        };
    }
} 