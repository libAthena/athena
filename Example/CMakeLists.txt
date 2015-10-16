if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
cmake_minimum_required(VERSION 3.0)
project(Example)
endif()

include_directories(${ATHENA_INCLUDE_DIR})
add_executable(Example
    main.cpp)

if(GEKKO)
include_directories(${LIBOGCDIR}/include)
link_directories(${LIGOGCDIR}/lib/wii)
target_link_libraries(Example AthenaCore z wiiuse bte ogc m)
else()
#TODO: add normal link libraries
endif()

