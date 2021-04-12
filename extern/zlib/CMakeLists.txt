if (APPLE)
# use toolchain zlib
find_package(ZLIB REQUIRED)
set(ZLIB_LIBRARIES ZLIB::ZLIB)
else()
if(NOT WIN32 AND NOT NX) # remove WIN32 when specter/freetype is gone
find_library(ZLIB_LIB NAMES zlib z)
endif()
if(NOT ZLIB_LIB)
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
if(WIN32 AND NOT UNIX)
install(FILES zconf.h zlib.h DESTINATION include COMPONENT zlib)
install(TARGETS z DESTINATION lib COMPONENT zlib)
else()
target_compile_options(z PRIVATE -Wno-implicit-fallthrough)
endif()
set(ZLIB_LIBRARIES z CACHE PATH "Zlib libraries" FORCE)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Zlib include path" FORCE)
else()
set(ZLIB_LIBRARIES ${ZLIB_LIB} CACHE PATH "Zlib libraries" FORCE)
find_path(ZLIB_INCLUDE_DIR zlib.h PATHS "${PNG_LIB}/../../include")
message(STATUS "Using zlib at ${ZLIB_LIB}, include: ${ZLIB_INCLUDE_DIR}")
endif()
endif()