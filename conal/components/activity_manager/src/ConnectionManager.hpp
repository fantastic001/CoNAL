#pragma once 

#include <thread>
#include <memory>
#include <list> 
#include <mutex>

#include <Connection.hpp>

namespace conal {
    namespace activity_manager {
        class ConnectionManager {
            std::mutex m_mutex;
            std::list< std::shared_ptr<::conal::framework::Connection>> connections;

        public:
            explicit ConnectionManager();
            void addConnection(std::shared_ptr<::conal::framework::Connection> connection);
            void removeClosed();
        };
    }
}