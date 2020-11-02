#include "LoaderManager.hpp" 
#include <filesystem>
#include <dlfcn.h>

using namespace ::conal::code_manager;


LoaderManager::LoaderManager(std::shared_ptr<::conal::framework::Logger> logger) {
    this->logger = logger;
    std::string loaderDir = std::string(std::getenv("CONAL_DIR")) + "/loaders/";
    for (auto& library : std::filesystem::directory_iterator(loaderDir)) {
        if (library.path().extension() == ".so") {
            logger->debug("Loading loader at " + std::string(library.path()));
            void* handle = dlopen(library.path().c_str(), RTLD_LAZY);
            if (!handle) {
                logger->error("Error while opening library: " + std::string(library.path()));
            }
            typedef Loader* (*createInstance_t)();
            createInstance_t createInstance = (createInstance_t) dlsym(handle, "createInstance");
            Loader* loader = createInstance();
            const char *dlsym_error = dlerror();
            if (dlsym_error) {
                logger->error(std::string(dlsym_error));
            }
            loaders.push_back(std::shared_ptr<Loader>(loader));
            dlclose(handle);

        }
    }

}

std::shared_ptr<Loader> LoaderManager::findLoader(std::string path, std::vector<std::string> params) {
    for (auto loader : loaders) {
        if (loader->validate(path, params)) {
            return loader;
        }
    }
    return nullptr;
}