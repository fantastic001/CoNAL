#pragma once

#include "Source.hpp"
#include <memory>
#include <string>
#include <vector>
#include <Logger.hpp>

namespace conal {
    namespace data_manager {
        /*
        This class is used to manage all sources present in environment. It does dynamic 
        loading of shared object file which implement particular sources. 

        Every source should implement Source class and provide function with the following
        signature:

        extern "C" Source* createInstance() {
            ...
        }
        */
        class SourceManager {
                std::shared_ptr<::conal::framework::Logger> logger;
                std::map<std::string, std::shared_ptr<Source>> sources;
            public: 
                /* 
                Construct SourceManager
                
                \param logger pointer to logger object from component class 
                */
                SourceManager(std::shared_ptr<::conal::framework::Logger> logger);

                /*
                Given path, parameters and information about environment, function finds suitable 
                source to load and run given data..
                */
                std::pair<std::string, std::shared_ptr<Source>> findSource(std::string path, std::vector<std::string> params, EnvParams env);


        };
    }
}