#pragma once 

#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <Connection.hpp>
namespace conal {
    namespace framework {
        class TCPServer {
            boost::asio::io_context io_context;
            std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptorPtr;
            std::shared_ptr<boost::asio::ip::tcp::endpoint> endpointPtr; 
            std::list<std::shared_ptr<Connection>> connections;
            int port;
            std::string bindAddress; 
            std::mutex mutex;

        public:
            TCPServer(std::string bindAddress, int port);
            template<typename ConnectionHandler, typename MessageHandler> 
            void run(ConnectionHandler c, MessageHandler m) {
                this->accept(c,m);
            }
            template<typename ConnectionHandler, typename MessageHandler> 
            void accept(ConnectionHandler c, MessageHandler m) {
                // std::cout << "Waiting for connection\n";
                acceptorPtr->async_accept([this, c, m] (boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                    std::shared_ptr<Connection> conn(new ::conal::framework::Connection(std::move(socket)));
                    c(conn);
                    this->addConnection(conn);
                    this->doRead(m,ec,conn);
                    this->accept(c,m);
                });
                try {
                    io_context.run();
                } catch (std::exception& e)
                {
                    std::cerr << "ERROR: " << e.what() << std::endl;
                }
            }

            void addConnection(std::shared_ptr<Connection> conn);

            template<typename MessageHandler> 
            void doRead(MessageHandler m, boost::system::error_code ec,std::shared_ptr<Connection> conn) {
                // std::cout << "Reading\n";
                auto buffer = std::make_shared<boost::asio::streambuf>();
                auto f = [this, conn, m, buffer] (boost::system::error_code ec, size_t n) {
                    // std::cout << "Here is it!\n";
                    std::string message;
                    std::istream is(buffer.get());
                    is >> message;
                    m(conn, message);
                    buffer->consume(n);
                    this->doRead(m,ec,conn);
                };
                async_read_until(*conn, *buffer, '\n', f);
            }
        };
    }
}