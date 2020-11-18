
install(PROGRAMS autostart.sh 
    DESTINATION components/data_manager/
)

set(DATA_MANAGER_SOURCES
    src/DataManagerComponent.cpp
    src/SourceManager.cpp
    src/DataDefinition.cpp
)

set(DATA_MANAGER_TEST_SOURCES
    test/main.cpp
)

add_executable(data_manager ${DATA_MANAGER_SOURCES} src/main.cpp)
target_link_libraries(data_manager PUBLIC ConalFramework dl)
target_include_directories(data_manager PUBLIC ../../framework/include)
target_include_directories(data_manager PUBLIC ./src)

# installing binaries to components/data_manager/bin for type TARGETS
install(TARGETS data_manager DESTINATION components/data_manager/bin)

add_executable(testDataManager ${DATA_MANAGER_TEST_SOURCES} ${DATA_MANAGER_SOURCES})
target_link_libraries(testDataManager gtest gmock dl)
target_link_libraries(testDataManager PUBLIC ConalFramework dl)
target_include_directories(testDataManager PUBLIC ../../framework/include)
target_include_directories(testDataManager PUBLIC ./src)


add_test(NAME test_data_manager COMMAND testDataManager)


set(LOADERS 
    DummySource
)

foreach(LOADER ${LOADERS})
    add_library(${LOADER} SHARED sources/${LOADER}/${LOADER}.cpp)
    target_link_libraries(${LOADER} PUBLIC ConalFramework dl)
    target_include_directories(${LOADER} PUBLIC src/ sources/${LOADER}/)
    install(TARGETS ${LOADER} DESTINATION sources/)
endforeach(LOADER)

