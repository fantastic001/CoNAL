#include <DataDefinition.hpp>

using namespace std; 
using namespace conal::data_manager; 

void ignore_all(string& code, char token) {
    while (code.at(0) == token) code.erase(0, 1);
}

string accept_string(string& code) {
    ignore_all(code, ' '); 
    string result = ""; 
    while (code.at(0) != ' ' && code.at(0) != '(' && code.at(0) != ',' && code.at(0) != ')'
      && code.at(0) != '=' && code.at(0) != '[' && code.at(0) != ']') {
          result += code.at(0);
          code.erase(0, 1);
    }
    return result; 
}

string accept_token(string& code, char token) {
    ignore_all(code, ' ');
    if (code.at(0) == token) {
        code.erase(0, 1);
        string res = "";
        res.push_back(token);
        return res;
    }
    return "";
}

bool next_token(string& code, char token) {
    ignore_all(code, ' ');
    if (code.at(0) == token) return true; 
    else return false; 
}

DataDefinition::DataDefinition(std::string specification) {
    name = accept_string(specification);
    ignore_all(specification, ' ');
    accept_token(specification, '(');
    while (! next_token(specification, ')')) {
        params.push_back(accept_string(specification));
        if (next_token(specification, ')')) break; 
        accept_token(specification, ',');
    }
    accept_token(specification, ')');
    if (next_token(specification, '[')) {
        accept_token(specification, '[');
        while (! next_token(specification, ']')) {
            string p = accept_string(specification);
            accept_token(specification, '=');
            string value = accept_string(specification);
            optional_params[p] = value;
            if (next_token(specification, ']')) break; 
            accept_token(specification, ',');
        }
    }

}

std::string DataDefinition::getName() const {
    return name; 
}
std::vector<std::string> DataDefinition::getParams() const {
    return params; 
}
std::map<std::string, std::string> DataDefinition::getOptionalParams() const {
    return optional_params;
}