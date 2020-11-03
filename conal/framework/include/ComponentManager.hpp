#pragma once 

#include "Component.hpp"
#include <memory>
#include "Logger.hpp"

namespace conal {
    namespace framework {
        class Logger;
        class ComponentManager {
            public:
                /* 
                Called by individual components to register themselves. 

                \param name Name given to the component 
                \param component Shared pointer to object implementing Component class
                */
                void registerComponent(std::string name, std::shared_ptr<Component> component);

                // Get instance of ComponentManager. ComponentManager is singleton.
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