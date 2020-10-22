#include <Connection.hpp>
#include <iostream>

namespace conal {
    namespace framework {
        Connection::Connection(boost::asio::ip::tcp::socket _socket)  : socket(std::move(_socket)) {
                
        }
        void Connection::send(std::string data) {
            boost::asio::streambuf buf;
            std::ostream os(&buf);
            os << data;
            boost::asio::write(socket, buf);
        }

        bool Connection::closed() {
            return ! socket.is_open();
        }
        std::string Connection::getHostname() {
            return socket.remote_endpoint().address().to_string();
        }

    }
}