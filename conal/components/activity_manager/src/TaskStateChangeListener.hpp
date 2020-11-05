#pragma once;

#include "TaskState.hpp"

namespace conal {
    namespace activity_manager {
        class Task; 
        class TaskStateChangeListener {
            public:
                virtual void onStateChange(Task& taskRef, TaskState oldState, TaskState newState) =0;
        };
    }
}