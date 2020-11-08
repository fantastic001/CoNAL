#include "LoaderManager.hpp" 
#include <filesystem>
#include <dlfcn.h>

using namespace ::conal::code_manager;


LoaderManager::LoaderManager(std::shared_ptr<::conal::framework::Logger> logger) {
    this->logger = logger;
    logger->debug("LoaderManager::LoaderManager");
    std::string loaderDir = std::string(std::getenv("CONAL_DIR")) + "/loaders/";
    for (auto& library : std::filesystem::directory_iterator(loaderDir)) {
        if (library.path().extension() == ".so") {
            logger->debug("Loading loader at " + std::string(library.path()));
            void* handle = dlopen(library.path().c_str(), RTLD_LAZY);
            if (!handle) {
                logger->error("Error while opening library: " + std::string(library.path()));
            }
            else logger->debug("Loader successfully opened");
            typedef Loader* (*createInstance_t)();
            createInstance_t createInstance = (createInstance_t) dlsym(handle, "createInstance");
            Loader* loader = createInstance();
            const char *dlsym_error = dlerror();
            if (dlsym_error) {
                logger->error(std::string(dlsym_error));
            }
            loaders[library.path().filename()]=(std::shared_ptr<Loader>(loader));
        }
    }

}

std::pair<std::string, std::shared_ptr<Loader>> LoaderManager::findLoader(std::string path, std::vector<std::string> params, EnvParams env) {
    for (auto loader : loaders) {
        logger->debug("Checking loader");
        if (loader.second->validate(path, params, env)) {
            logger->debug("Found loader!");
            return loader;
        }
    }
    logger->debug("Loader not found by LoaderManager");
    return std::make_pair("", nullptr);
}
void LoaderManager::start(std::string loaderName, std::string code) {
    logger->info("Starting program with loader " + loaderName);
    loaders[loaderName]->run(code);
}
