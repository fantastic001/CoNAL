#pragma once 

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

namespace conal {
    namespace framework {
        /*
        Class used to connect to server instance using TCPServer object.
        */
        class TCPClient {
                boost::asio::io_service io_service;
                boost::asio::ip::tcp::resolver resolver;
                std::shared_ptr<boost::asio::ip::tcp::socket> socketPtr; 


            public:
                /*
                Construct object and connect to server. 

                \param hostname hostname of node to connect to 
                \param service name of service or port like "6969" or "ftp".
                */
                explicit TCPClient(std::string hostname, std::string service); 

                /*
                Send data to server
                Function automatically serializes data.
                
                \param data data object which can be used with ostream with operator "<<".
                */
                template <typename DataType> 
                void send(DataType data) {
                    boost::asio::streambuf buf;
                    std::ostream os(&buf);
                    os << data;
                    os << "\n";
                    boost::asio::write(*socketPtr, buf);
                }
                
                /*
                Read data from TCP stream until newline or EOF is encountared.

                \return string representing read line (without \n character).
                */
                std::string readLine();

                /*
                same as readLine except result is deserialized. 

                \return deserialized object
                */
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