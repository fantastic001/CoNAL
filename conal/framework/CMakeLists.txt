set(FRAMEWORK_SOURCES 
    src/ComponentManager.cpp 
    src/Logger.cpp 
    src/Performative.cpp
)



add_library(ConalFramework SHARED ${FRAMEWORK_SOURCES}

)

target_include_directories(ConalFramework PUBLIC include/)
target_compile_features(ConalFramework PUBLIC cxx_std_17)
target_link_libraries(ConalFramework PUBLIC Threads::Threads)