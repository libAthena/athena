find_package(ZLIB)
if(NOT ZLIB_FOUND)
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
install(FILES zconf.h zlib.h DESTINATION include)
install(TARGETS z DESTINATION lib)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Zlib include path" FORCE)
endif()
