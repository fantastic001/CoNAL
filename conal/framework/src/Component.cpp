
#include <Component.hpp>
#include <fstream>
#include <Message.hpp>

using namespace ::conal::framework;
void Component::sendMessage(std::string to_component, Performative performative, std::string body) {
        auto os = std::fstream(
            std::string(std::getenv("COMPONENT_COMM_DIR")) + "/" + to_component + "/messages", 
            std::ios::out
        );
        os << Message(performative, name, to_component, body);
}
