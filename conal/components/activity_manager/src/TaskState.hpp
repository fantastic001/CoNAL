#pragma once;

namespace conal {
    namespace activity_manager {
        enum class TaskState {
            CREATED,
            PREPARING,
            PREPARED,
            LOADING,
            LOADED,
            SENDING, 
            SENT, 
            READY,
            RUNNING,
            ERROR,
            FINISHED
        }; 
    }
}