cmake_minimum_required (VERSION 2.6)

project (CoNAL)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.42.0 REQUIRED COMPONENTS system thread regex)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

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
