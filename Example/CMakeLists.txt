if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
cmake_minimum_required(VERSION 3.0)
project(Example)
endif()

add_executable(Example
    main.cpp)

if(GEKKO)
include_directories(${LIBOGCDIR}/include)
link_directories(${LIGOGCDIR}/lib/wii)
target_link_libraries(Example athena-core z wiiuse bte ogc m)
target_include_directories(Example PRIVATE ${LIBOGCDIR}/include)
target_link_directories(Example PRIVATE ${LIBOGCDIR}/lib/wii)
else()
#TODO: add normal link libraries
endif()

