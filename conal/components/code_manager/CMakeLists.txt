
install(PROGRAMS autostart.sh 
    DESTINATION components/code_manager/
)

set(CODE_MANAGER_SOURCES
    src/main.cpp
    src/CodeManagerComponent.cpp
    src/LoaderManager.cpp
)

set(CODE_MANAGER_TEST_SOURCES
    test/main.cpp
)

add_executable(code_manager ${CODE_MANAGER_SOURCES})
target_link_libraries(code_manager PUBLIC ConalFramework dl)
target_include_directories(code_manager PUBLIC ../../framework/include)
target_include_directories(code_manager PUBLIC ./src)

# installing binaries to components/code_manager/bin for type TARGETS
install(TARGETS code_manager DESTINATION components/code_manager/bin)

add_executable(testCodeManager ${CODE_MANAGER_TEST_SOURCES})
target_link_libraries(testCodeManager gtest gmock dl)

add_test(NAME test_code_manager COMMAND testCodeManager)


set(LOADERS 
    DummyLoader
    CppLoader
)

foreach(LOADER ${LOADERS})
    add_library(${LOADER} SHARED loaders/${LOADER}/${LOADER}.cpp)
    target_link_libraries(${LOADER} PUBLIC ConalFramework dl)
    target_include_directories(${LOADER} PUBLIC src/ loaders/${LOADER}/)
    install(TARGETS ${LOADER} DESTINATION loaders/)
endforeach(LOADER)

