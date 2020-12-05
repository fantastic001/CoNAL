#pragma once 

#include <vector> 
#include <string>
#include <map>
#include <optional>
#include <utility>
#include <memory>

using EnvParams = std::map<std::string, std::string>;

namespace conal {
    namespace data_manager {
        class Source;
        using SourceCopy = std::optional<std::shared_ptr<Source>>;
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
                virtual SourceCopy init(std::vector<std::string> params, std::map<std::string, std::string> optional_params) =0;

                /*
                This function fetches one element of data source givn parameters from init.
                */
                virtual std::string getOne()=0;

                // this function is implemented also by individual source implementations
                // it has to return true if end of data stream in encouraged
                // for instance, for TextFile source, this returns true when 
                // EOF is found. 
                virtual bool end() =0; 

                /*
                This function splits this source in two smaller sources, probably of the same type.
                This function should call init method to initialize new source.
                */
                virtual std::pair<std::shared_ptr<Source>, std::shared_ptr<Source>> split() =0;

                /*
                This function is aimed for serializing source such that it can 
                be transfered over network. It is usually base64 encoded string. Serialized 
                string should not contain any whitspace or newline or tab. 
                */
               virtual std::string serialize()=0;

               /*
               This function is called instead of init to initialize source with parameters
               when source is transfered over network. 
               */
               virtual SourceCopy deserialize(std::string code) =0;

               /*
               This function should output parsable data source representation in data 
               definition language
               */
              virtual std::string dump() =0;

              // Adds data represented in a string to data instance and returns true if data is added
              virtual bool add(std::string) =0;

              // Retrieves data given key, otherwise empty
              virtual std::optional<std::string> at(std::string) =0;

               virtual ~Source() {
                    
               }
        };
    }
}