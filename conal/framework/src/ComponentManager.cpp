#include <ComponentManager.hpp>
#include <memory>
#include <string>
#include <utility>
#include <POSIXPipe.hpp>
#include <sstream>

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
    logger.info("Starting component " + name);
    component->start();
    component.get()->messageReadingThread = std::thread([&component, &name] () {
        POSIXPipe pipe(3, std::ios::in);
        std::istream& is = pipe.getStream();
        std::string msg;
        while (std::getline(is, msg)) {
            std::stringstream ss(msg);
            Message message; 
            ss >> message.performative;
            ss >> message.from_component;
            ss >> message.to_component;
            std::getline(ss, message.body);
            component->handleMessage(message);
        }
    });
    component.get()->messageReadingThread.join();
    
}
