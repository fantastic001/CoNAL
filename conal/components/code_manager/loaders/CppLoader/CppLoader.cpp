#include "CppLoader.hpp"
#include <filesystem> 
#include <fstream>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

#include <sstream>
#include <chrono>
#include <thread>

#include <dlfcn.h>
#include <iostream>


using namespace conal::code_manager;

std::string get_random_executable_name() {
    const auto p1 = std::chrono::system_clock::now();
    std::stringstream ss; 
    ss << std::chrono::duration_cast<std::chrono::seconds>(
                   p1.time_since_epoch()).count() ;
    return "/tmp/conal-code-manager-cpploader-output-"+ss.str() + ".so";
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

std::string read_file(std::string path) {
    std::fstream reader(path);
    std::stringstream ss; 
    while (reader.good()) {
        std::string line;
        std::getline(reader, line);
        ss << line << std::endl;
    }
    reader.close();
    return ss.str();
}

std::string conal_entry_function = "___conal_entry_cpp";

std::string CppLoader::load(std::string path, std::vector<std::string> params, EnvParams env) {
    auto toolchainPath = findToolChain(env);
    auto execName = get_random_executable_name();
    std::string source_path = get_random_executable_name() + ".cpp";
    
    std::fstream source_output(source_path, std::ios::out);
    source_output 
        << read_file(path)
        << std::endl
        << "#include <vector>\n"
        << "#include <DataBinding.hpp>\n"
        << "typedef int (*entry_start_pointer_t)(std::vector<::conal::code_manager::DataBinding>,std::vector<::conal::code_manager::DataBinding>);\n"
        << "extern \"C\" " 
        << "entry_start_pointer_t " 
        << conal_entry_function << "()" 
        << "{"
        << "return start;"
        << "}"
        << std::endl;
    source_output.close();

    std::string command = toolchainPath + " " + source_path + " -shared " + "-o " + execName;
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
void CppLoader::run(std::string code, std::vector<DataBinding> in, std::vector<DataBinding> out) {
    auto execName = get_random_executable_name();
    std::ofstream executableOutput(execName, std::ofstream::out | std::ofstream::binary);
    auto result = decode(code);
    executableOutput.write(result.data(), result.size());
    executableOutput.close();
    std::string command1 = "chmod +x " + execName;
    void* handle = dlopen(execName.c_str(), RTLD_LAZY);
    const char *dlsym_error = dlerror();
    if (!handle) {
        std::cout << "Error while loading file: " << execName << std::endl;
        std::cout << dlsym_error << std::endl;
        return;
    }
    typedef int (*entry_start_pointer_t)(std::vector<::conal::code_manager::DataBinding>,std::vector<::conal::code_manager::DataBinding>);
    
    typedef entry_start_pointer_t (*entry_function)();
    entry_function entry_symbol = (entry_function) dlsym(handle, conal_entry_function.c_str());
    dlsym_error = dlerror();
    if (dlsym_error) {
            std::cout << dlsym_error << std::endl;
    }
    entry_start_pointer_t start_function = entry_symbol();
    std::thread process([start_function, in, out] () {
        int result = start_function(in, out);
    });
    process.detach();
    
}

std::string CppLoader::findToolChain(EnvParams env) {
    std::string conalDirectory = std::string(std::getenv("CONAL_DIR")); 
    std::string flags =  " -L" + conalDirectory + "/sdk/lib/"
            + " -lConalCodeManager -lConalFramework"
            + " -I" + conalDirectory + "/sdk/include/"
            + " -fPIC";
    if (env["ARCH"] == "x86_64" && fs::exists("/usr/bin/g++")) return "/usr/bin/g++ " + flags;
    if (fs::exists("/usr/bin/" + env["ARCH"] + "-linux-gnu-g++")) {
        return "/usr/bin/" + env["ARCH"] + "-linux-gnu-g++ " + flags;
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