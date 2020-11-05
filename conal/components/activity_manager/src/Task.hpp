#pragma once;

#include <Connection.hpp>
#include <list>
#include "TaskState.hpp"
#include "TaskStateChangeListener.hpp"

namespace conal {
    namespace activity_manager {
        class Task {
                std::list<std::shared_ptr<::conal::framework::Connection>> connections;
                std::string path;
                std::vector<std::string> params; 
                std::list<std::shared_ptr<TaskStateChangeListener>> listeners;
                TaskState state; 
            public:
                Task(std::string path, std::vector<std::string> params);
                void prepare(); 
                void load();
                void start(); 
                void addListener(std::shared_ptr<TaskStateChangeListener> listener);

                void setState(TaskState state);
                TaskState getState() const;

        };
    }
}