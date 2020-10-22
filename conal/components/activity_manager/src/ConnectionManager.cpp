
#include "ConnectionManager.hpp"


using namespace ::conal::framework;
using namespace std;

conal::activity_manager::ConnectionManager::ConnectionManager() {
}

void conal::activity_manager::ConnectionManager::addConnection(shared_ptr<Connection> connection) {
    unique_lock<mutex> lock(m_mutex);
    connections.push_back(connection);
}

void conal::activity_manager::ConnectionManager::removeClosed() {
    unique_lock<mutex> lock(m_mutex);
    list<shared_ptr<Connection>> newConnectionList; 
    for (auto iter = connections.begin(); iter != connections.end(); iter++) {
        if (! (*iter)->closed() && (*iter)->canPing()) {
            newConnectionList.push_back(*iter);
        }
    }
    connections = newConnectionList;
}

list<shared_ptr<Connection>> conal::activity_manager::ConnectionManager::getConnections() const {
    return connections;
}