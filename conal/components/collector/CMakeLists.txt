

set(COLLECTOR_SOURCES
src/CollectorComponent.cpp
)


add_executable(collector ${COLLECTOR_SOURCES} src/main.cpp)
target_link_libraries(collector PUBLIC ConalFramework dl)
target_include_directories(collector PUBLIC ../../framework/include)
target_include_directories(collector PUBLIC ./src)

# installing binaries to components/code_manager/bin for type TARGETS
install(TARGETS collector DESTINATION components/collector/bin)
