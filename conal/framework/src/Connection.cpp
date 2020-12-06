#include <Connection.hpp>
#include <iostream>

#include <NetworkUtils.hpp>

namespace conal {
    namespace framework {
        Connection::Connection(int _socket, sockaddr* addr)
            : sockfd(_socket), addr(addr)
        {
        }
        void Connection::send(std::string data) {
            std::unique_lock<std::mutex> lock(send_mutex);
            size_t len = data.size();
            conal::utilities::send_all(sockfd, &len, sizeof(size_t), 0);
            conal::utilities::send_all(sockfd, data.c_str(), len, 0);
        }

        bool Connection::closed() {
            return sockfd == 0;
        }
        std::string Connection::getHostname() {
            char str[INET6_ADDRSTRLEN];
            if (addr->sa_family == AF_INET) {
                inet_ntop(
                    AF_INET,
                    &((sockaddr_in*)addr)->sin_addr,
                    str,
                    INET_ADDRSTRLEN
                );
            } else {
                inet_ntop(
                    AF_INET6,
                    &((sockaddr_in6*)addr)->sin6_addr,
                    str,
                    INET6_ADDRSTRLEN
                );
            }
            return std::string(str);
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