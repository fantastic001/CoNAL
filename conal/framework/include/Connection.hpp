#pragma once 

#include <boost/asio.hpp>

namespace conal {
    namespace framework {
        class TCPServer;
        class Connection {
            friend class TCPServer;
            boost::asio::ip::tcp::socket socket;
            std::map<std::string, std::string> properties; 

            Connection(boost::asio::ip::tcp::socket socket);

        public:
            typedef boost::asio::ip::tcp::socket::executor_type executor_type;
            void send(std::string data);
            bool closed();
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

            bool canPing();

            void setProperty(std::string property, std::string value);
            std::string getProperty(std::string property) const;

        };
    }
}