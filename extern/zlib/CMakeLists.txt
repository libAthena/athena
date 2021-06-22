if (APPLE)
  # always use toolchain zlib
  set(ZLIB_REQUIRED REQUIRED)
endif ()
find_package(ZLIB ${ZLIB_REQUIRED})
if (ZLIB_FOUND)
  add_library(z INTERFACE)
  target_link_libraries(z INTERFACE ${ZLIB_LIBRARIES})
  target_include_directories(z INTERFACE ${ZLIB_INCLUDE_DIRS})
  set(ZLIB_LIBRARIES z CACHE STRING "zlib libraries" FORCE)
  set(ATHENA_ZLIB NO CACHE BOOL "Using built-in zlib" FORCE)
else ()
  message(STATUS "Using Athena's built-in zlib")
  add_library(z
      adler32.c
      compress.c
      crc32.c
      deflate.c
      infback.c
      inffast.c
      inflate.c
      inftrees.c
      trees.c
      uncompr.c
      zutil.c
      crc32.h
      deflate.h
      gzguts.h
      inffast.h
      inffixed.h
      inflate.h
      inftrees.h
      trees.h
      zconf.h
      zlib.h
      zutil.h)
  if (WIN32 AND NOT UNIX)
    install(FILES zconf.h zlib.h DESTINATION include COMPONENT zlib)
    install(TARGETS z DESTINATION lib COMPONENT zlib)
  else ()
    target_compile_options(z PRIVATE -Wno-implicit-fallthrough)
  endif ()
  target_include_directories(z INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
  set(ZLIB_LIBRARIES z CACHE STRING "zlib libraries" FORCE)
  set(ATHENA_ZLIB YES CACHE BOOL "Using built-in zlib" FORCE)
endif ()
