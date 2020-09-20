cmake_minimum_required (VERSION 2.6)

project (CoNAL)

install(PROGRAMS setup-env.sh DESTINATION . 
)

set(components 
    activity_manager
)

foreach(component ${components})

    add_subdirectory(components/${component}/ components/${component})

endforeach()

add_subdirectory(env/ env/)