#pragma once 

#include <vector> 
#include <string>
#include <map>
#include <optional>

using EnvParams = std::map<std::string, std::string>;

namespace conal {
    namespace data_manager {
        using ErrorValue = std::optional<std::string>;
        class Source {
            public:
                /*
                Every source is given set of parameters. The init method should handle these
                parameters in source-specific manner and store them in its memory. 
                For instance, for TextFile, there is parameter of file path. Each source 
                should also document its parameters for users to use it properly. 

                There is also way of passing optional parameters, for instance:
                    Range(0, ...)[step=1.0] where there are two parameters and one optional:
                        1. "0"
                        2. "..."
                        Optional: "step" with value "1.0"
                
                This function returns Optional which is empty if there is no error and with 
                string value if is present, where string value describes error.
                */
                virtual ErrorValue init(std::vector<std::string> params, std::map<std::string, std::string> optional_params) =0;

                /*
                This function fetches one element of data source givn parameters from init.
                */
                virtual std::string getOne()=0;

                // this function is implemented also by individual source implementations
                // it has to return true if end of data stream in encouraged
                // for instance, for TextFile source, this returns true when 
                // EOF is found. 
                virtual bool end() =0; 
                
                virtual ~Source() {
                    
                }
        };
    }
}