
#include <Logger.hpp>
#include <chrono>

using namespace conal::framework; 

Logger::Logger(std::string name)  : logFile("log/" + name + ".log"), componentName(name) {
}

void Logger::log(int level, std::string message) {
    std::lock_guard<std::mutex> lock(mutex); 
    logFile << std::chrono::system_clock::now().time_since_epoch().count()
        << " " 
        << componentName << ": " << message << std::endl;

}

void Logger::debug(std::string message) {
    log(4, message);
}

void Logger::info(std::string message) {
    log(3, message);
}

void Logger::warning(std::string message) {
    log(2, message);
}

void Logger::error(std::string message) {
    log(1, message);
}

void Logger::critical(std::string message) {
    log(0, message);
}