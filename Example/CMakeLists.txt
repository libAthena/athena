cmake_minimum_required(VERSION 3.0)
project(Example)

include_directories(${ATHENA_INCLUDE_DIR})
add_executable(Example
    main.cpp)

if(GEKKO)
target_link_libraries(Example AthenaCore z wiiuse bte ogc m)
else()
#TODO: add normal link libraries
endif()

