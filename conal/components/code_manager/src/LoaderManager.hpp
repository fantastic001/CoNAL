#pragma once

#include "Loader.hpp"
#include <memory>
#include <string>
#include <vector>
#include <Logger.hpp>

namespace conal {
    namespace code_manager {
        /*
        This class is used to manage all loaders present in environment. It does dynamic 
        loading of shared object file which implement particular loaders. 

        Every loader should implement Loader class and provide function with the following
        signature:

        extern "C" Loader* createInstance() {
            ...
        }
        */
        class LoaderManager {
                std::shared_ptr<::conal::framework::Logger> logger;
                std::map<std::string, std::shared_ptr<Loader>> loaders;
            public: 
                /* 
                Construct LoaderManager
                
                \param logger pointer to logger object from component class 
                */
                LoaderManager(std::shared_ptr<::conal::framework::Logger> logger);

                /*
                Given path, parameters and information about environment, function finds suitable 
                loader to load and run given code..
                */
                std::pair<std::string, std::shared_ptr<Loader>> findLoader(std::string path, std::vector<std::string> params, EnvParams env);
                void start(std::string loaderName, std::string code);


        };
    }
}