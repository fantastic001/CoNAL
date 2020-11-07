#pragma once;

#include <Connection.hpp>
#include <list>
#include "TaskState.hpp"
#include "TaskStateChangeListener.hpp"

namespace conal {
    namespace activity_manager {
        class Task {
                std::list<std::shared_ptr<::conal::framework::Connection>> connections;
                std::map<std::string, std::string> connectionHostnameToCodeMap;
                std::string path;
                std::vector<std::string> params; 
                std::list<std::shared_ptr<TaskStateChangeListener>> listeners;
                TaskState state; 
            public:
                Task(std::string path, std::vector<std::string> params);
                void addConnection(std::shared_ptr<::conal::framework::Connection> connection);
                void prepare(std::shared_ptr<::conal::framework::Connection> connection); 
                void load(std::shared_ptr<::conal::framework::Connection> connection, std::string code);
                void start(); 
                void addListener(std::shared_ptr<TaskStateChangeListener> listener);

                void setState(TaskState state);
                TaskState getState() const;

                bool isLoadable();
                bool isPrepared();
                std::string getCode(std::shared_ptr<::conal::framework::Connection>);

                std::list<std::shared_ptr<::conal::framework::Connection>> getConnections() const;


        };
    }
}