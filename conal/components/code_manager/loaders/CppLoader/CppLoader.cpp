#include "CppLoader.hpp"
#include <filesystem> 
#include <fstream>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

#include <sstream>
#include <chrono>

std::string get_random_executable_name() {
    const auto p1 = std::chrono::system_clock::now();
    std::stringstream ss; 
    ss << std::chrono::duration_cast<std::chrono::seconds>(
                   p1.time_since_epoch()).count() ;
    return "/tmp/conal-code-manager-cpploader-output-"+ss.str();
}

std::string decode64(const std::string &val) {
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
    return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(val)), It(std::end(val))), [](char c) {
        return c == '\0';
    });
}

std::string encode64(const std::string &val) {
    using namespace boost::archive::iterators;
    using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
    auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
    return tmp.append((3 - val.size() % 3) % 3, '=');
}

namespace fs = std::filesystem;


bool CppLoader::validate(std::string path, std::vector<std::string> params, EnvParams env) {
    return ! findToolChain(env).empty()
        && fs::path(path).extension() == ".cpp";
}
std::string CppLoader::load(std::string path, std::vector<std::string> params, EnvParams env) {
    auto toolchainPath = findToolChain(env);
    auto execName = get_random_executable_name();
    std::string command = toolchainPath + " " + path + " " + "-o " + execName;
    int status = system(command.c_str());
    if (status != 0) return "";
    std::string output;
    std::ifstream executableReader(execName, std::ifstream::binary);
    while (executableReader.good()) {
        char byteRead; 
        executableReader.read(&byteRead, 1);
        output.push_back(byteRead);
    }
    auto code = encode(output);
    return code; 
}
void CppLoader::run(std::string code) {
    auto execName = get_random_executable_name();
    std::ofstream executableOutput(execName, std::ofstream::out | std::ofstream::binary);
    auto result = decode(code);
    executableOutput.write(result.data(), result.size());
    executableOutput.close();
    std::string command1 = "chmod +x " + execName;
    system(command1.c_str());
    system(execName.c_str());
}

std::string CppLoader::findToolChain(EnvParams env) {
    std::string conalDirectory = std::string(std::getenv("CONAL_DIR")); 
    if (env["ARCH"] == "x86_64" && fs::exists("/usr/bin/g++")) return "/usr/bin/g++";
    if (fs::exists("/usr/bin/" + env["ARCH"] + "-linux-gnu-g++")) {
        return "/usr/bin/" + env["ARCH"] + "-linux-gnu-g++";
    }
    return "";
}

std::string CppLoader::encode(std::string a) {
    return encode64(std::string(a));
}
std::string CppLoader::decode(std::string a) {
    return decode64(a);
}

extern "C" ::conal::code_manager::Loader* createInstance() {
    return new CppLoader();
}