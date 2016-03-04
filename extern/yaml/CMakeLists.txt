set(YAML_VERSION_MAJOR 0)
set(YAML_VERSION_MINOR 1)
set(YAML_VERSION_PATCH 6)
set(YAML_VERSION_STRING "${YAML_VERSION_MAJOR}.${YAML_VERSION_MINOR}.${YAML_VERSION_PATCH}")

file(GLOB SRC src/*.c)

include_directories(../../include)
add_library(athena-libyaml STATIC ${SRC})

install(TARGETS athena-libyaml DESTINATION lib COMPONENT yaml)
