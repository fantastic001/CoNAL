#include <Message.hpp>
#include <iostream>

using namespace conal::framework;

std::ostream& ::conal::framework::operator<<(std::ostream& out, const Message& message) {
    out << message.reply_with << " " << message.performative << " " << message.from_component 
        << " " << message.to_component << " " << message.body <<  "\n";
    return out;
}
std::istream& ::conal::framework::operator>>(std::istream& ss,conal::framework::Message& message) {
    ss >> message.reply_with;
    ss >> message.performative;
    ss >> message.from_component;
    ss >> message.to_component;
    std::getline(ss, message.body);
    int beginCharIndex = 0; 
    while (message.body[beginCharIndex] == ' ' && beginCharIndex < message.body.size()) beginCharIndex++;
    message.body = message.body.substr(beginCharIndex);
    return ss; 
}