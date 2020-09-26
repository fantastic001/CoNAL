#pragma once 

#include <fstream>
#include <mutex>

namespace conal {
    namespace framework {
        class Logger {
            friend class ComponentManager; 
                std::ofstream logFile; 
                std::string componentName;
                std::mutex mutex;
            public:
                void debug(std::string message);
                void info(std::string message);
                void warning(std::string message);
                void error(std::string message);
                void critical(std::string message);
            private:
                Logger(std::string name);
                void log(int level, std::string message);
        };
    }
}