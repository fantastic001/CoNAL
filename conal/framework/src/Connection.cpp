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
            os << "\n";
            boost::asio::write(socket, buf);
        }

        bool Connection::closed() {
            return ! socket.is_open();
        }
        std::string Connection::getHostname() {
            return socket.remote_endpoint().address().to_string();
        }

        bool Connection::canPing() {
            try {
                send("ping");
            }
            catch (boost::wrapexcept<boost::system::system_error> e) {
                return false;
            }
            return true;
        }

        void Connection::setProperty(std::string property, std::string value) {
            properties[property] = value; 
        }

        std::string Connection::getProperty(std::string property) const {
            if (properties.find(property) != properties.end()) {
                return properties.find(property)->second;
            }
            else {
                return "";
            }
        }

    }
}