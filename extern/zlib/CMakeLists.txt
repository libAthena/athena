if(NOT WIN32)
find_package(ZLIB)
endif()
if(WIN32 OR NOT ZLIB_FOUND)
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
endif()
set(ZLIB_LIBRARIES z CACHE PATH "Zlib libraries" FORCE)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Zlib include path" FORCE)
else()
set(ZLIB_LIBRARIES ${ZLIB_LIBRARIES} CACHE PATH "Zlib libraries" FORCE)
find_path(ZLIB_INCLUDE_DIR zlib.h)
endif()
