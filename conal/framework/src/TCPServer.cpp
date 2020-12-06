
#include <TCPServer.hpp>
#include <memory>
#include <utility>
#include <sstream>

using namespace boost::asio;

namespace conal
{
    namespace framework
    {
        TCPServer::TCPServer(std::string bindAddress, int port)
            : bindAddress(bindAddress), port(port)
        {
            struct addrinfo hints;
            struct addrinfo *result, *rp;
            char buf[500];
            std::stringstream s;
            s << port;

            memset(&hints, 0, sizeof(struct addrinfo));
            hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
            hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
            hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */
            hints.ai_protocol = 0;           /* Any protocol */

            if (getaddrinfo(bindAddress.c_str(), s.str().c_str(), &hints, &result) != 0)
            {
                // TODO: could not resolve hostname
                return;
            }

            for (rp = result; rp != NULL; rp = rp->ai_next)
            {
                sockfd = socket(rp->ai_family, rp->ai_socktype,
                                rp->ai_protocol);
                if (sockfd == -1)
                    continue;

                if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
                    break; /* Success */

                close(sockfd);
                sockfd = 0;
            }

            if (rp == NULL)
            {
                // TODO: none of the addresses were reachable
                return;
            }

            freeaddrinfo(result);
        }

        void TCPServer::addConnection(std::shared_ptr<Connection> conn)
        {
            connections.push_back(std::move(conn));
        }
    } // namespace framework
} // namespace conal