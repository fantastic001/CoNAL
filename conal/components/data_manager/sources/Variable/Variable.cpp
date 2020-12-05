
#include <Variable.hpp>
#include <utility>

using namespace conal::data_manager;

SourceCopy Variable::init(std::vector<std::string> params, std::map<std::string, std::string> optional_params) {
    auto instance = std::make_shared<Variable>();
    if (params.size() == 0) instance->value = "";
    else instance->value = params[0];
    return SourceCopy(instance);
}


std::string Variable::getOne() {
    return value;
}


bool Variable::end() {
    return false; 
}

std::pair<std::shared_ptr<Source>, std::shared_ptr<Source>> Variable::split() {
    auto first = std::make_shared<Variable>();
    auto second = std::make_shared<Variable>();
    first->init(std::vector<std::string>(), std::map<std::string, std::string>());
    second->init(std::vector<std::string>(), std::map<std::string, std::string>());
    return std::make_pair(first, second); 
}


std::string Variable::serialize() {
    return value;
}

std::string Variable::dump() {
    if (value.empty()) return "Variable()";
    else return "Variable(" + value + ")";
}

conal::data_manager::SourceCopy Variable::deserialize(std::string code) {
    auto instance = std::make_shared<Variable>();
    instance->value = code;
    return SourceCopy(instance);
}


bool Variable::add(std::string data) {
    value = data;
    return true; 
}
std::optional<std::string> Variable::at(std::string key) {
    return std::optional(value);
}


extern "C" Source* createInstance() {
    return new Variable();
}