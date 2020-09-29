
#include <TCPServer.hpp>
#include <memory>
#include <utility>

using namespace boost::asio;

namespace conal {
    namespace framework {
        TCPServer::TCPServer(std::string bindAddress, int port) : bindAddress(bindAddress), port(port) 
        {
            endpointPtr = std::make_shared<boost::asio::ip::tcp::endpoint>(boost::asio::ip::tcp::v4(),  port);
            acceptorPtr = std::make_shared<boost::asio::ip::tcp::acceptor>(io_context, *endpointPtr);
            
        }
        void TCPServer::addConnection(std::shared_ptr<Connection> conn) {
            connections.push_back(std::move(conn));
        }
    }
}