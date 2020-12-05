#pragma once 

#include <Component.hpp>
#include <Message.hpp>
#include <vector> 
#include <string>
#include <LoaderManager.hpp> 
#include <DataManagerCommunicationInterface.hpp>
#include <future>
#include <memory>

namespace conal {
    namespace code_manager {
        class CodeManagerComponent : 
            public conal::framework::Component, 
            public DataManagerCommunicationInterface,
            std::enable_shared_from_this<CodeManagerComponent>
            {
                using LoaderPtr = std::pair<std::string, std::shared_ptr<Loader>>;
                using LoadingResult = std::pair<std::string, LoaderPtr>;
                
                template<typename T> 
                using FutureReplyMap = std::map<int, std::promise<T>>;
                std::shared_ptr<::conal::code_manager::LoaderManager> loaderManager;
                
                std::map<std::pair<int, std::string>, LoadingResult> taskIdHostnameToCodeMapping;
                EnvParams getEnvParams();

                FutureReplyMap<std::string> replyAt;
                FutureReplyMap<std::string> replyGet;
                FutureReplyMap<bool> replyEnd;
                FutureReplyMap<bool> replyAdd;

                DataBindings dataInstances; 


                template<typename T> 
                T sendRequestToDataManager(std::string identifier, std::string request, FutureReplyMap<T>& replyMap, std::string key = "") {
                    using Performative = conal::framework::Performative;
                    std::promise<T> my_promise; 
                    auto my_future = my_promise.get_future();
                    int replyNum = sendReplyableMessage("data_manager", Performative::REQUEST, 
                        request + " " + identifier + (!key.empty() ? " " + key : "")
                    );
                    replyMap[replyNum] = std::move(my_promise);
                    return my_future.get();
                }

            public: 
                explicit CodeManagerComponent();
                virtual void start();
                virtual void stop();
                virtual void handleMessage(::conal::framework::Message msg);



                std::string sendGetRequest(std::string identifier);
                std::string sendAtRequest(std::string identifier, std::string key);
                bool sendEndRequest(std::string identifier);
                bool sendAddRequest(std::string identifier, std::string data);

        };
    }
}