#include "Task.hpp"

using namespace conal::activity_manager;

Task::Task(std::string path, std::vector<std::string> params) 
: path(path), params(params) {
    state = TaskState::CREATED;
}

void Task::addConnection(std::shared_ptr<::conal::framework::Connection> connection) {
    connections.push_back(connection);
}

void Task::prepare(std::shared_ptr<::conal::framework::Connection> connection) {
    connectionHostnameToCodeMap[connection->getHostname()] = "";
    if (isPrepared()) setState(TaskState::PREPARED);
}

void Task::load(std::shared_ptr<::conal::framework::Connection> connection, std::string code) {
    if (state != TaskState::LOADING) return;
    connectionHostnameToCodeMap[connection->getHostname()] = code;
    if (isLoadable()) setState(TaskState::LOADED);

}
void Task::start() {
    if (state != TaskState::LOADED) return;
    if (!isLoadable()) return;
}
void Task::addListener(std::shared_ptr<TaskStateChangeListener> listener) {
    listeners.push_back(listener);
}

void Task::setState(TaskState state) {
    auto oldState = this->state;
    this->state = state;
    for (auto listener : listeners) {
        listener->onStateChange(*this, oldState, this->state);
    }
}
TaskState Task::getState() const {
    return state;
}

bool Task::isPrepared() {
    if (connections.size() == connectionHostnameToCodeMap.size()) return true; 
    else return false;
}

bool Task::isLoadable() {
    if (isPrepared()) {
        bool isok = true;
        for (auto it : connectionHostnameToCodeMap) {
            if (it.second.empty()) isok = false;
        }
        return isok;
    }
    else return false;
}
std::list<std::shared_ptr<::conal::framework::Connection>> Task::getConnections() const {
    return connections;
}

std::string Task::getCode(std::shared_ptr<::conal::framework::Connection> connection) {
    return connectionHostnameToCodeMap[connection->getHostname()];
}
