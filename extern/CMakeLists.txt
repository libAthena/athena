add_subdirectory(lzokay)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(lzokay PRIVATE -Wno-maybe-uninitialized)
endif ()
add_subdirectory(zlib)
add_subdirectory(yaml)
if(NOT TARGET fmt)
  add_subdirectory(fmt)
  target_compile_definitions(fmt PUBLIC
          FMT_ARM_ABI_COMPATIBILITY=1
          FMT_EXCEPTIONS=0)
  target_compile_definitions(fmt INTERFACE
          FMT_ENFORCE_COMPILE_STRING=1)
endif()