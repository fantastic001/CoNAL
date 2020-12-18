
#include <Array.hpp>
#include <utility>
#include <algorithm>
#include <sstream>

using namespace conal::data_manager;

SourceCopy Array::init(std::vector<std::string> params, std::map<std::string, std::string> optional_params) {
    auto instance = std::make_shared<Array>();
    instance->value = params;
    instance->i = 0;
    return SourceCopy(instance);
}


std::string Array::getOne() {
    if (i < value.size()) return value[i++];
    else return "";
}


bool Array::end() {
    return i == value.size();
}

std::pair<std::shared_ptr<Source>, std::shared_ptr<Source>> Array::split() {
    std::vector<std::string> firstValue; 
    std::vector<std::string> secondValue; 
    if (value.size() > 1) {
        std::copy(value.begin(), value.begin() + value.size()/2, firstValue.begin());
        std::copy(value.begin() + value.size()/2,value.end(), secondValue.begin());
    }
    
    auto first = init(firstValue, std::map<std::string, std::string>());
    auto second = init(secondValue, std::map<std::string, std::string>());
    return std::make_pair(first.value(), second.value()); 
}


std::string Array::serialize() {
    std::stringstream ss; 
    for (auto e : value) {
        ss << e << "###";
    }
    return ss.str();
}

std::string Array::dump() {
    std::stringstream ss; 
    ss << "Array(";
    int k;
    for (k = 0; k<value.size()-1; k++) {
        ss << value[k] << ",";
    }
    ss << value[k] << ")";
    return ss.str();
}

std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

conal::data_manager::SourceCopy Array::deserialize(std::string code) {
    auto instance = std::make_shared<Array>();
    std::stringstream ss(ReplaceAll(code, "###", " "));
    while (ss.good()) {
        std::string elem; 
        ss >> elem; 
        instance->value.push_back(elem);
    }
    return SourceCopy(instance);
}


bool Array::add(std::string data) {
    value.push_back(data);
    return true; 
}
std::optional<std::string> Array::at(std::string key) {
    std::stringstream ss(key);
    int k; 
    ss >> k;
    if (k >= 0 && k < value.size()) {
        return std::optional(value[k]);
    }
    else return std::optional<std::string>();
}


extern "C" Source* createInstance() {
    return new Array();
}