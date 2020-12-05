#pragma once 


#include <DataChangeListener.hpp>
#include <mutex>
#include <condition_variable>
#include <DataBinding.hpp>

using params = conal::code_manager::DataBindings;

namespace conal {
    namespace utilities {
        class data_waiter : public conal::code_manager::DataChangeListener {
                std::mutex m; 
                std::condition_variable cv; 

                void onDataChange(conal::code_manager::DataBinding& db) {
                    std::unique_lock<std::mutex> lock(m);
                    cv.notify_all();
                }
            public:
                void wait() {
                    std::unique_lock<std::mutex> lock(m);
                    cv.wait(lock);
                }
        };

        void wait_for_data(::conal::code_manager::DataBinding& db) {
            auto waiter = std::make_shared<data_waiter>();
            db.listen(waiter);
            waiter->wait();
        }
    }
}
