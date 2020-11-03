
#include <CodeManagerComponent.hpp>
#include <iostream>
#include <sstream>

using namespace conal::code_manager; 
using namespace conal::framework;

CodeManagerComponent::CodeManagerComponent() {
}
void CodeManagerComponent::start() {
    logger->info("Starting Code manager component");
    loaderManager = std::make_shared<LoaderManager>(logger);
}

void CodeManagerComponent::stop() {

}

void CodeManagerComponent::handleMessage(Message msg) {
    logger->debug("Got message: " + msg.body);
    if (msg.performative == Performative::REQUEST) {
        std::stringstream ss(msg.body);
        std::string command; 
        ss >> command;
        if (command == "load") {
            logger->debug("Rest of the message body: " + msg.body);
            std::string path;
            ss >> path;
            std::vector<std::string> params; 
            while (ss.good()) {
                std::string param; 
                ss >> param;
                params.push_back(param);
            }
            logger->info("Loading code " + path);
            for (auto p : params) {
                logger->debug(p);
            }
            EnvParams env = getEnvParams();
            auto loader = loaderManager->findLoader(path, params, env);
            if (!loader) {
                logger->error("No suitable loader for " + path);
            }
            else {
                logger->info("Loading " + path);
                std::string code = loader->load(path, params, env);
                logger->info("Running " + path);
                loader->run(code);
            }
        }
    }
}

EnvParams CodeManagerComponent::getEnvParams() {
    return EnvParams();
}