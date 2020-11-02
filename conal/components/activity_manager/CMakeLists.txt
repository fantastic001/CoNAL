
install(PROGRAMS autostart.sh 
    DESTINATION components/activity_manager/
)

set(ACTIVITY_MANAGER_SOURCES
    src/main.cpp
    src/ActivityManagerComponent.cpp
    src/ConnectionManager.cpp
)

set(ACTIVITY_MANAGER_TEST_SOURCES
    test/main.cpp
)

add_executable(activity_manager ${ACTIVITY_MANAGER_SOURCES})
target_link_libraries(activity_manager PUBLIC ConalFramework)
target_include_directories(activity_manager PUBLIC ../../framework/include)
target_include_directories(activity_manager PUBLIC ./src)

# installing binaries to components/activity_manager/bin for type TARGETS
install(TARGETS activity_manager DESTINATION components/activity_manager/bin)

add_executable(testActivityManager ${ACTIVITY_MANAGER_TEST_SOURCES})
target_link_libraries(testActivityManager gtest gmock)

add_test(NAME test_activity_manager COMMAND testActivityManager)
