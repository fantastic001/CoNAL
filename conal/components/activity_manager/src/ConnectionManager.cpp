
#include "ConnectionManager.hpp"


using namespace ::conal::framework;
using namespace std;

conal::activity_manager::ConnectionManager::ConnectionManager() {
    thread([] () {
        while (true) {
            

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void conal::activity_manager::ConnectionManager::addConnection(shared_ptr<Connection> connection) {
    unique_lock<mutex> lock(m_mutex);
    connections.push_back(connection);
}

void conal::activity_manager::ConnectionManager::removeClosed() {
    unique_lock<mutex> lock(m_mutex);
    std::list<decltype(connections.begin())> to_remove; 
    for (auto iter = connections.begin(); iter != connections.end(); iter++) {
        if ((*iter)->closed()) {
            to_remove.push_back(iter);
        }
    }
    for (auto iter = to_remove.begin(); iter != to_remove.end(); ++iter) {
        connections.erase(*iter);
    }
}