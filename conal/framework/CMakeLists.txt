set(FRAMEWORK_SOURCES 
    src/ComponentManager.cpp 
)



add_library(ConalFramework ${FRAMEWORK_SOURCES}

)
target_include_directories(ConalFramework PUBLIC include/)
target_compile_features(ConalFramework PUBLIC cxx_std_17)