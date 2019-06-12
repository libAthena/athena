set(YAML_VERSION_MAJOR 0)
set(YAML_VERSION_MINOR 1)
set(YAML_VERSION_PATCH 6)
set(YAML_VERSION_STRING "${YAML_VERSION_MAJOR}.${YAML_VERSION_MINOR}.${YAML_VERSION_PATCH}")

file(GLOB SRC src/*.c)

add_library(athena-libyaml STATIC ${SRC})
target_include_directories(athena-libyaml PUBLIC $<BUILD_INTERFACE:${athena_SOURCE_DIR}/include>)

install(TARGETS athena-libyaml DESTINATION lib EXPORT AthenaTargets COMPONENT yaml)

if (NOT MSVC)
  target_compile_options(athena-libyaml PRIVATE -Wno-unused-but-set-variable -Wno-unused-value)
endif()
