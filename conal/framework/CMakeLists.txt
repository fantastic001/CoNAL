set(FRAMEWORK_SOURCES 
    src/ComponentManager.cpp 
    src/Logger.cpp 
    src/Performative.cpp
    src/TCPServer.cpp
    src/Connection.cpp
    src/TCPClient.cpp
    src/Component.cpp
    src/Message.cpp
)
# Waiting for CMake 3.19 for this feature 
# file(REAL_PATH include/ FRAMEWORK_INCLUDE_DIRECTORY)


set(FRAMEWORK_HEADERS
    ${FRAMEWORK_INCLUDE_DIRECTORY}/Component.hpp
    ${FRAMEWORK_INCLUDE_DIRECTORY}/ComponentManager.hpp
    ${FRAMEWORK_INCLUDE_DIRECTORY}/Connection.hpp
    ${FRAMEWORK_INCLUDE_DIRECTORY}/Logger.hpp
    ${FRAMEWORK_INCLUDE_DIRECTORY}/Message.hpp
    ${FRAMEWORK_INCLUDE_DIRECTORY}/Performative.hpp
    ${FRAMEWORK_INCLUDE_DIRECTORY}/POSIXPipe.hpp
    ${FRAMEWORK_INCLUDE_DIRECTORY}/TCPCLient.hpp
    ${FRAMEWORK_INCLUDE_DIRECTORY}/TCPServer.hpp
)
set(FRAMEWORK_TEST_SOURCES
        test/main.cpp
    )

# first we can indicate the documentation build as an option and set it to ON by default
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN Doxyfile.in)
    set(DOXYGEN_OUT Doxyfile.out)
    string(REPLACE ";" " " DOXYGEN_HEADER_INPUT "${FRAMEWORK_HEADERS}")
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT})


    # note the option ALL which allows to build the docs together with the application
    add_custom_target( doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY .
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)


add_library(ConalFramework SHARED ${FRAMEWORK_SOURCES}

)

target_include_directories(ConalFramework PUBLIC include/)
target_compile_features(ConalFramework PUBLIC cxx_std_17)
target_link_libraries(ConalFramework PUBLIC Threads::Threads)

install(TARGETS ConalFramework DESTINATION lib/)

option(FRAMEWORK_TESTS "Build the tests" ON)
if(FRAMEWORK_TESTS)
    
    add_executable(testFramework ${FRAMEWORK_TEST_SOURCES})
    target_link_libraries(testFramework gtest gmock)

    add_test(NAME test_framework COMMAND testFramework)

    
endif()