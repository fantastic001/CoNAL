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
    component->name = name;
    logger.info("Starting component " + name);
    component->start();
    component.get()->messageReadingThread = std::thread([&component, &name] () {;
        std::string msg;
        auto is = std::fstream(std::getenv("COMPONENT_MSG_FIFO"), std::ios::in | std::ios::out);
        while (! std::getline(is, msg).eof()) {
            std::stringstream ss(msg);
            Message message; 
            ss >> message.performative;
            ss >> message.from_component;
            ss >> message.to_component;
            std::getline(ss, message.body);
            int beginCharIndex = 0; 
            while (message.body[beginCharIndex] == ' ') beginCharIndex++;
            message.body = message.body.substr(beginCharIndex);
            component->handleMessage(message);
        }
    });
    component.get()->messageReadingThread.join();
    
}
