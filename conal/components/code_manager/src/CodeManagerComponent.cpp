
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
    if (msg.from_component == "data_manager" && msg.performative == Performative::NOTIFY) {
        std::stringstream ss(msg.body);
        std::string id;
        std::string data; 
        ss >> id;
        for (int i = 0; i<dataInstances.size(); i++) {
            if (dataInstances[i].getIdentifier() == id) {
                dataInstances[i].notify();
            }
        }
    }
    else if (msg.from_component == "data_manager" && msg.performative == Performative::REPLY) {
        int num = msg.reply_with;
        if (replyGet.find(num) != replyGet.end()) replyGet[num].set_value(msg.body);
        if (replyAt.find(num) != replyAt.end()) replyAt[num].set_value(msg.body);
        if (replyEnd.find(num) != replyEnd.end()) replyEnd[num].set_value(msg.body == "1");
        if (replyAdd.find(num) != replyAdd.end()) replyAdd[num].set_value(msg.body == "1");

    }
    else if (msg.performative == Performative::REQUEST && msg.from_component == "data_manager")
    {
        ::std::stringstream ss(msg.body);
        std::string command; 
        ss >> command;
        if (command == "binding") {
            std::string id; 
            ss >> id;
            dataInstances.push_back(DataBinding(id, this));
        }
    }
    else if (msg.performative == Performative::REQUEST) {
        std::stringstream ss(msg.body);
        std::string hostname;
        int id; 
        ss >> id >> hostname;
        std::string env; 
        ss >> env;
        EnvParams envParams; 
        while (env.find("=") > 0 && env.find("=") < env.size()-1 && ss.good()) {
            std::string envName = env.substr(0, env.find('='));
            std::string envVal =  env.substr(env.find('=')+1, env.size());
            logger->debug("Got environment var: " + envName + "=" + envVal);
            envParams[envName] =envVal;
            ss >> env;
        }
        auto path = env;
        logger->debug("path=" + path);
        logger->debug("Rest of the message body: " + msg.body);
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
        std::stringstream outputWriter; 
        outputWriter << id << " " << hostname;
        if (envParams.empty()) envParams = getEnvParams();
        auto loader = loaderManager->findLoader(path, params, envParams);
        if (!loader.second) {
            logger->error("No suitable loader for " + path);
            sendMessage("activity_manager", Performative::REJECT, outputWriter.str());
        }
        else {
            sendMessage("activity_manager", Performative::ACCEPT, outputWriter.str());
            logger->info("Loading " + path);
            std::string code = loader.second->load(path, params, envParams);
            taskIdHostnameToCodeMapping[std::make_pair(id, hostname)] = std::make_pair(code, loader);
            
        }
    }
    else if (msg.performative == Performative::DATA) {
        std::string body = msg.body;
        std::stringstream ss(msg.body);
        int id; 
        std::string hostname;
        ss >> id >> hostname;
        auto code = taskIdHostnameToCodeMapping[std::make_pair(id, hostname)].first;
        std::string loaderName = taskIdHostnameToCodeMapping[std::make_pair(id, hostname)].second.first;
        logger->debug("Sending code for " + ss.str() + " for loader " + loaderName + " CODE=" + code);
        sendMessage("activity_manager", Performative::DATA, body + " " + code + " " + loaderName);
    }
    else if (msg.performative == Performative::START) {
        std::string loaderName, code;
        std::stringstream ss(msg.body);
        ss >> loaderName;
        ss >> code;
        loaderManager->start(loaderName, code, dataInstances, dataInstances);
    }
}

EnvParams CodeManagerComponent::getEnvParams() {
    return EnvParams();
}



std::string CodeManagerComponent::sendGetRequest(std::string identifier) {
    return sendRequestToDataManager<std::string>(identifier, "get", replyGet);
}

std::string CodeManagerComponent::sendAtRequest(std::string identifier, std::string key) {
    return sendRequestToDataManager<std::string>(identifier, "at",replyAt, key);
}
bool CodeManagerComponent::sendEndRequest(std::string identifier) {
    return sendRequestToDataManager<bool>(identifier, "end", replyEnd);
}
bool CodeManagerComponent::sendAddRequest(std::string identifier, std::string data) {
    return sendRequestToDataManager<bool>(identifier, "add", replyAdd, data);
}