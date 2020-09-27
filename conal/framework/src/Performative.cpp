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
    return in; 

}