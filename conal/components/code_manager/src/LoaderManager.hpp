#pragma once

#include "Loader.hpp"
#include <memory>
#include <string>
#include <vector>
#include <Logger.hpp>

namespace conal {
    namespace code_manager {
        class LoaderManager {
                std::shared_ptr<::conal::framework::Logger> logger;
                std::vector<std::shared_ptr<Loader>> loaders; 
            public:
                LoaderManager(std::shared_ptr<::conal::framework::Logger> logger);
                std::shared_ptr<Loader> findLoader(std::string path, std::vector<std::string> params);


        };
    }
}