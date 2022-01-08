#pragma once 

#include <boost/asio.hpp>
#include <map>

namespace conal {
    namespace framework {
        class TCPServer;
        /*
        \brief Connection is constructed by TCPServer object to manage connections.
        */
        class Connection {
            friend class TCPServer;
            std::mutex send_mutex;
            std::mutex read_mutex;
            int sockfd;
            std::map<std::string, std::string> properties;
            sockaddr *addr;

            Connection(int _socket, sockaddr* addr);

        public:
            /*
            \brief Send data to connection represented by object.
            \param data String data to be sent
            */
            void send(std::string data);

            /*
            Returns true if connection is closed.
            \return true if connection is closed.
            */
            bool closed();

            /*
            Returns hostname of connection 
            \return string representing hostname in form of ipv4 or ipv6 address
            */
            std::string getHostname();
            
            /*
            Sends ping message using send(...) function and checks if message is delivered. 
            This does not wait for pong response. 

            \return true if message was able to be sent.
            */
            bool canPing();

            /*
            Every connection object has its map of properties. For instance, different 
            nodes in network can contain various information which may be important like
            architecture and so on. This function sets property for given connection. 
            Concrete named of properties are not defined and they are implementation specific
            as well as way to sending these informations across nodes in network.

            \param property name of property to be set
            \param value string representation of value
            */
            void setProperty(std::string property, std::string value);

            /* 
            Get property for particular connection
            If property is not set, empty string is returned. 

            \return  string representation of value for particular property.
            */
            std::string getProperty(std::string property) const;

        };
    }
}