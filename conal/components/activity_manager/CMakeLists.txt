
set(CMAKE_BUILD_TYPE Debug)

install(PROGRAMS autostart.sh 
    DESTINATION components/activity_manager/
)

set(ACTIVITY_MANAGER_SOURCES
    src/ActivityManagerComponent.cpp
    src/ConnectionManager.cpp
    src/Task.cpp
)

set(ACTIVITY_MANAGER_TEST_SOURCES
    test/main.cpp
)

find_package(BISON)
find_package(FLEX)
BISON_TARGET(MyParser src/node_spec/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(MyScanner src/node_spec/lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(activity_manager 
    ${ACTIVITY_MANAGER_SOURCES}
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
    src/main.cpp
)

target_link_libraries(activity_manager PUBLIC ConalFramework)
target_include_directories(activity_manager PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/)
target_include_directories(activity_manager PUBLIC ../../framework/include)
target_include_directories(activity_manager PUBLIC ./src)
target_link_libraries(activity_manager PUBLIC ${FLEX_LIBRARIES})

# installing binaries to components/activity_manager/bin for type TARGETS
install(TARGETS activity_manager DESTINATION components/activity_manager/bin)

add_executable(testActivityManager ${ACTIVITY_MANAGER_TEST_SOURCES}
    ${ACTIVITY_MANAGER_SOURCES}
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
target_link_libraries(testActivityManager PUBLIC ConalFramework)
target_include_directories(testActivityManager PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/)
target_include_directories(testActivityManager PUBLIC ../../framework/include)
target_include_directories(testActivityManager PUBLIC ./src)
target_link_libraries(testActivityManager PUBLIC ${FLEX_LIBRARIES})
target_link_libraries(testActivityManager gtest gmock)

add_test(NAME test_activity_manager COMMAND testActivityManager)
