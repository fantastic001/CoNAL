cmake_minimum_required (VERSION 2.6)

project (CoNAL)

install(PROGRAMS setup-env.sh DESTINATION . 
)

find_package(Threads REQUIRED)

set(components 
    activity_manager
)

foreach(component ${components})

    add_subdirectory(components/${component}/ components/${component})

endforeach()

add_subdirectory(env/ env/)
add_subdirectory(framework/ framework/)
