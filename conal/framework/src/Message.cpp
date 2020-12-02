#include <Message.hpp>
#include <iostream>

using namespace conal::framework;

std::ostream& ::conal::framework::operator<<(std::ostream& out, const Message& message) {
    out << message.reply_with << message.performative << " " << message.from_component 
        << " " << message.to_component << " " << message.body <<  "\n";
    return out;
}