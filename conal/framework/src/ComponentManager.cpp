#include <ComponentManager.hpp>
#include <memory>
#include <string>
#include <utility>
#include <POSIXPipe.hpp>
#include <sstream>
#include <list>

using namespace conal::framework;

std::shared_ptr<ComponentManager> ComponentManager::instance = std::shared_ptr<ComponentManager>(nullptr);

ComponentManager::ComponentManager() : logger("ComponentManager") {

}

std::shared_ptr<ComponentManager> ComponentManager::getInstance() {
    if (!instance) instance = std::shared_ptr<ComponentManager>(new ComponentManager());
    return instance;
}

void ComponentManager::registerComponent(std::string name, std::shared_ptr<Component> component) {
    logger.info("Registering component " + name);
    component->logger = std::shared_ptr<Logger>(new Logger(name));
    component->name = name;
    logger.info("Starting component " + name);
    component->start();
    component.get()->messageReadingThread = std::thread([&logger = logger, &component, &name] () {;
        std::string msg;
        auto is = std::fstream(std::getenv("COMPONENT_MSG_FIFO"), std::ios::in | std::ios::out);
        while (! std::getline(is, msg).eof()) {
            std::stringstream ss(msg);
            Message message;
            ss >> message;
            logger.debug("Delivering message: " + message.body + " from " + message.from_component);
            component->deliver(message);
        }
    });
    component.get()->messageReadingThread.join();
    
}
