#include <ComponentManager.hpp>
#include <memory>
#include <string>
#include <utility>

using namespace conal::framework;


ComponentManager::ComponentManager() {

}

std::shared_ptr<ComponentManager> ComponentManager::getInstance() {
    if (!instance) instance = std::shared_ptr<ComponentManager>(new ComponentManager());
    return instance;
}

void ComponentManager::registerComponent(std::string name, std::shared_ptr<Component> component) {
}
