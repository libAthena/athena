set(YAML_VERSION_MAJOR 0)
set(YAML_VERSION_MINOR 1)
set(YAML_VERSION_PATCH 6)
set(YAML_VERSION_STRING "${YAML_VERSION_MAJOR}.${YAML_VERSION_MINOR}.${YAML_VERSION_PATCH}")

file(GLOB SRC src/*.c)

include_directories(include win32)
add_library(AthenaLibYaml STATIC ${SRC} include/yaml.h)

set(YAML_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "YAML include path" FORCE)
#install(DIRECTORY include/ DESTINATION include/Athena COMPONENT yaml)
install(TARGETS AthenaLibYaml DESTINATION lib COMPONENT yaml)
