#include "Task.hpp"

using namespace conal::activity_manager;

Task::Task(std::string path, std::vector<std::string> params) 
: path(path), params(params) {
    state = TaskState::CREATED;
}
void Task::prepare() {

}
void Task::load() {

}
void Task::start() {

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