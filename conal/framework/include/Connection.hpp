#pragma once 

#include <boost/asio.hpp>

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
            boost::asio::ip::tcp::socket socket;
            std::map<std::string, std::string> properties; 

            Connection(boost::asio::ip::tcp::socket socket);

        public:
            typedef boost::asio::ip::tcp::socket::executor_type executor_type;
            
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

            auto get_executor() -> decltype(socket.get_executor()) {
                return socket.get_executor();
            }

            template <typename MutableBufferSequence,
                BOOST_ASIO_COMPLETION_TOKEN_FOR(void (boost::system::error_code,
                    std::size_t)) ReadHandler
                    BOOST_ASIO_DEFAULT_COMPLETION_TOKEN_TYPE(executor_type)>
            BOOST_ASIO_INITFN_AUTO_RESULT_TYPE(ReadHandler,
                void (boost::system::error_code, std::size_t))
            async_read_some(const MutableBufferSequence & buffers, ReadHandler&& handler) 
            {
                return socket.async_read_some(buffers, handler);
            }
            
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