#pragma once

#include "CodeManagerComponent.hpp"
#include "Loader.hpp"

namespace conal {
    namespace code_manager {
        class LoaderManager {
            friend class CodeManagerComponent;
                LoaderManager();
            public:
                std::shared_ptr<Loader> findLoader(std::string path, std::vector<std::string> params);


        };
    }
}