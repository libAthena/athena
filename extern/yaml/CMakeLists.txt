# Minimal CMake project for building a static library under Windows.

cmake_minimum_required(VERSION 2.8)
project(yaml C)

set(YAML_VERSION_MAJOR 0)
set(YAML_VERSION_MINOR 1)
set(YAML_VERSION_PATCH 6)
set(YAML_VERSION_STRING "${YAML_VERSION_MAJOR}.${YAML_VERSION_MINOR}.${YAML_VERSION_PATCH}")

file(GLOB SRC src/*.c)

include_directories(include win32)
add_definitions(-DYAML_DECLARE_STATIC)
add_library(yaml STATIC ${SRC} include/yaml.h)

set(YAML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "YAML include path" FORCE)
if(WIN32 AND NOT UNIX)
  install(DIRECTORY include/ DESTINATION include COMPONENT yaml)
  install(TARGETS yaml DESTINATION lib COMPONENT yaml)
endif()
