
set(CODE_MANAGER_SOURCES
    CodeManagerComponent.cpp
)


add_executable(ping ${CODE_MANAGER_SOURCES} ping.cpp)
target_link_libraries(ping ConalFramework)
target_include_directories(ping PUBLIC ../../../framework/include)


add_executable(pong ${CODE_MANAGER_SOURCES} pong.cpp)
target_link_libraries(pong ConalFramework )
target_include_directories(pong PUBLIC ../../../framework/include)

