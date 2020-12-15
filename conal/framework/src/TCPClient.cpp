#include <TCPCLient.hpp> 

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <NetworkUtils.hpp>

using namespace ::conal::framework; 
using namespace std; 


namespace conal {
    namespace framework {
        TCPClient::TCPClient(std::string hostname, std::string service) {
            
            struct addrinfo hints;
            struct addrinfo *result, *rp;
            char buf[500];
            
            memset(&hints, 0, sizeof(struct addrinfo));
            hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
            hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
            hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
            hints.ai_protocol = 0;          /* Any protocol */
            
            if (getaddrinfo(hostname.c_str(), service.c_str(), &hints, &result) != 0)
            {
                // TODO: could not resolve hostname
                return;
            }
            
            for (rp = result; rp != NULL; rp = rp->ai_next) {
                sockfd = socket(rp->ai_family, rp->ai_socktype,
                            rp->ai_protocol);
                if (sockfd == -1)
                    continue;

                if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
                        break;                  /* Success */

                close(sockfd);
                sockfd = 0;
            }
            
            if (rp == NULL) {
                // TODO: none of the addresses were reachable
                return;
            }
            
            freeaddrinfo(result);
        }

        std::string TCPClient::readLine()  {
            std::unique_lock<std::mutex> lock(read_mutex);
            conal::utilities::packet_length_t len;
            conal::utilities::recv_all(
                sockfd,
                &len,
                sizeof(conal::utilities::packet_length_t),
                0
            ); // TODO: check rv for errors
            len = ntohl(len);
            char* buf = new char[len];
            conal::utilities::recv_all(sockfd, buf, len, 0); // TODO: check rv for errors
            std::string result(buf, buf+len);
            delete[] buf;
            return result;
        }
        
        void TCPClient::sendLine(const std::string& data) {
            std::unique_lock<std::mutex> lock(send_mutex);
            conal::utilities::packet_length_t len = htonl(data.size());
            conal::utilities::send_all(
                sockfd,
                &len,
                sizeof(conal::utilities::packet_length_t),
                0
            ); // TODO: check rv for errors
            conal::utilities::send_all(sockfd, data.c_str(), data.size(), 0); // TODO: check rv for errors
        }
    }
}
