#pragma once 

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace conal {
    namespace framework {
        class TCPClient {
                boost::asio::io_service io_service;
                boost::asio::ip::tcp::resolver resolver;
                std::shared_ptr<boost::asio::ip::tcp::socket> socketPtr; 


            public:
                explicit TCPClient(std::string hostname, std::string service); 
                template <typename DataType> 
                void send(DataType data) {
                    boost::asio::streambuf buf;
                    std::ostream os(&buf);
                    os << data;
                    os << "\n";
                    boost::asio::write(*socketPtr, buf);
                }
                
                std::string readLine();

                template<typename ResultType> 
                ResultType get() {
                    std::stringstream ss(readLine());
                    ResultType result; 
                    ss >> result;
                    return result; 
                }
        };
    }
}