#include "CppLoader.hpp"
#include <filesystem> 
#include <fstream>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

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
    std::string command = toolchainPath + " " + path + " " + "-o /tmp/output";
    int status = system(command.c_str());
    if (status != 0) return "";
    std::string output;
    std::ifstream executableReader("/tmp/output", std::ifstream::binary);
    while (executableReader.good()) {
        char byteRead; 
        executableReader.read(&byteRead, 1);
        output.push_back(byteRead);
    }
    auto code = encode(output);
    return code; 
}
void CppLoader::run(std::string code) {
    std::ofstream executableOutput("/tmp/output", std::ofstream::out | std::ofstream::binary);
    auto result = decode(code);
    executableOutput.write(result.data(), result.size());
    executableOutput.close();
    system("chmod +x /tmp/output");
    system("/tmp/output");
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