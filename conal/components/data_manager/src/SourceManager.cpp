#include "SourceManager.hpp" 
#include <filesystem>
#include <dlfcn.h>

using namespace ::conal::data_manager;


SourceManager::SourceManager(std::shared_ptr<::conal::framework::Logger> logger) {
    this->logger = logger;
    logger->debug("SourceManager::SourceManager");
    std::string sourceDir = std::string(std::getenv("CONAL_DIR")) + "/sources/";
    for (auto& library : std::filesystem::directory_iterator(sourceDir)) {
        if (library.path().extension() == ".so") {
            logger->debug("Loading source at " + std::string(library.path()));
            void* handle = dlopen(library.path().c_str(), RTLD_LAZY);
            if (!handle) {
                logger->error("Error while opening library: " + std::string(library.path()));
            }
            else logger->debug("Source successfully opened");
            typedef Source* (*createInstance_t)();
            createInstance_t createInstance = (createInstance_t) dlsym(handle, "createInstance");
            Source* source = createInstance();
            const char *dlsym_error = dlerror();
            if (dlsym_error) {
                logger->error(std::string(dlsym_error));
            }
            sources[library.path().filename()]=(std::shared_ptr<Source>(source));
        }
    }

}

std::pair<std::string, std::shared_ptr<Source>> SourceManager::findSource(std::string name, std::vector<std::string> params, std::map<std::string, std::string> optional_params) {
    auto source = sources.find(name);
    if (source != sources.end()) {
        auto error_result = source->second->init(params, optional_params);
        if (!error_result) // no errors 
        {
            return *source;
        }
        else {
            logger->error("ERROR: in init(...) for " + source->first + ": " +  error_result.value());
            return std::make_pair("", nullptr);
        }
    }
    else {
        logger->debug("Source not found by SourceManager");
        return std::make_pair("", nullptr);
    }
    return std::make_pair("", nullptr);
}
