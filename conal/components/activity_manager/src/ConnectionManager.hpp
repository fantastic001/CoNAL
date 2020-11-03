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

            // Add new connection to this manager
            void addConnection(std::shared_ptr<::conal::framework::Connection> connection);

            // remove all connections if they are closed or cannot be pinged
            void removeClosed();

            // get list of all managed connections 
            std::list<std::shared_ptr<::conal::framework::Connection>> getConnections() const; 
        };
    }
}