
#include "ConnectionManager.hpp"


using namespace ::conal::framework;
using namespace std;
namespace ns = conal::activity_manager::node_spec;
namespace am = conal::activity_manager;
namespace cf = conal::framework;

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
std::list< std::shared_ptr<cf::Connection>> am::ConnectionManager::select(ns::Specification* spec) {
    list<std::shared_ptr<cf::Connection>> result; 
    for (auto conn : connections) {
        if (spec->valid(conn)) {
            result.push_back(conn);
        }
    }
    return result;
}
