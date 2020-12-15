#pragma once 

#include <iostream>
#include <sstream>
#include <mutex>

#include <sys/types.h>
#include <sys/socket.h>


namespace conal {
    namespace framework {
        /*
        Class used to connect to server instance using TCPServer object.
        */
        class TCPClient {
                int sockfd;

                std::mutex read_mutex;
                std::mutex send_mutex;


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
                    std::stringstream ss;
                    ss << data;
                    sendLine(ss.str());
                }
                
                /*
                Read data from TCP stream until newline or EOF is encountared.

                \return string representing read line (without \n character).
                */
                std::string readLine();
                void sendLine(const std::string& data);

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