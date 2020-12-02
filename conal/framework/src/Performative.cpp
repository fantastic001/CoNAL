#include "Performative.hpp"

using namespace ::conal::framework;

std::istream& ::conal::framework::operator>> (std::istream& in, Performative& perf) {
    std::string text; 
    in >> text;
    if (text == "REQUEST") perf = Performative::REQUEST;
    if (text == "START") perf = Performative::START;
    if (text == "ACCEPT") perf = Performative::ACCEPT;
    if (text == "CREATE") perf = Performative::CREATE;
    if (text == "STOP") perf = Performative::STOP;
    if (text == "REJECT") perf = Performative::REJECT;
    if (text == "DESTROY") perf = Performative::DESTROY;
    if (text == "DATA") perf = Performative::DATA;
    if (text == "REPLY") perf = Performative::REPLY;
    return in; 

}


std::ostream& ::conal::framework::operator<< (std::ostream& in, const Performative& perf) {
    if (perf == Performative::REQUEST) in << "REQUEST";
    if (perf == Performative::START) in << "START";
    if (perf == Performative::ACCEPT) in << "ACCEPT";
    if (perf == Performative::CREATE) in << "CREATE";
    if (perf == Performative::STOP) in << "STOP";
    if (perf == Performative::REJECT) in << "REJECT";
    if (perf == Performative::DESTROY) in << "DESTROY";
    if (perf == Performative::DATA) in << "DATA";
    if (perf == Performative::REPLY) in << "REPLY";
    return in; 

}