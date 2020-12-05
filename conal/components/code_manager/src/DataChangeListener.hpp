#pragma once

namespace conal {
    namespace code_manager {
        class DataBinding;
        class DataChangeListener {
            public:
            virtual void onDataChange(DataBinding& dataBinding) = 0;
        };
    }
}