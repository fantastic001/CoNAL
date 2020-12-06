#pragma once 

#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include <utility>
#include <thread>

#include <Connection.hpp>
#include <NetworkUtils.hpp>

namespace conal {
    namespace framework {
        class TCPServer {
            std::list<std::shared_ptr<Connection>> connections;
            int port;
            std::string bindAddress;
            std::mutex mutex;
            
            int sockfd;

        public:
            /*
            Create TCPServer instance.
            */
            TCPServer(std::string bindAddress, int port);
            
            /*
            Start listening on bindAddress and port given in constructor. 

            NOTE: c should accept std::shared_ptr<Connection> 
            NOTE: m should accept std::shared_ptr<Connection> and std::string

            \param c callable object which is called when new connection is connected. 
            \param m callable object which is called when message is sent from some connection


            */
            template<typename ConnectionHandler, typename MessageHandler> 
            void run(ConnectionHandler c, MessageHandler m) {
                std::thread listener([=]() {
                    this->accept(c, m);
                });
                
                listener.detach();
            }

            template<typename ConnectionHandler, typename MessageHandler> 
            void accept(ConnectionHandler c, MessageHandler m) {
                listen(sockfd, 123);
                
                while (true) {
                    sockaddr * addr = (sockaddr*)new sockaddr_in6();
                    socklen_t len = sizeof(sockaddr_in6);
                    int clientsock = ::accept(sockfd, addr, &len);
                    std::shared_ptr<Connection> conn(new Connection(clientsock, addr));
                    c(conn);
                    this->addConnection(conn);
                    std::thread reader([this, m, conn]() {
                        this->doRead(m, conn);
                    });
                    reader.detach();
                }

            }

            void addConnection(std::shared_ptr<Connection> conn);

            template<typename MessageHandler> 
            void doRead(MessageHandler m, std::shared_ptr<Connection> conn) {
                while (true) {
                    size_t len;
                    conal::utilities::recv_all(conn->sockfd, &len, sizeof(size_t), 0);
                    char * buffer = new char[len];
                    conal::utilities::recv_all(conn->sockfd, buffer, len, 0);
                    
                    std::string message(buffer, buffer+len);
                    m(conn, message);
                    delete[] buffer;
                }
            }
        };
    }
}