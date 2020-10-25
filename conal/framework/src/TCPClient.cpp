#include <TCPCLient.hpp> 

using namespace ::conal::framework; 
using namespace std; 

namespace conal {
    namespace framework {
        TCPClient::TCPClient(std::string hostname, std::string service) : resolver(io_service){
            boost::asio::ip::tcp::resolver::query query(hostname, service);
            auto it = resolver.resolve(query); 
            boost::asio::ip::tcp::resolver::iterator end; 
            socketPtr = std::make_shared<boost::asio::ip::tcp::socket>(io_service);
            boost::system::error_code error = boost::asio::error::host_not_found; 
            while (error && it != end)
            {
                socketPtr->close();
                socketPtr->connect(*(it++), error);
            }

        }

        std::string TCPClient::readLine()  {
            auto buffer = std::make_shared<boost::asio::streambuf>();
            auto n = boost::asio::read_until(*socketPtr, *buffer, "\n");
            std::string message;
            std::istream is(buffer.get());
            std::getline(is, message);
            buffer->consume(n);
            return message;

        }
    }
}
