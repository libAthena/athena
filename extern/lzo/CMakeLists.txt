if(NOT WIN32)
find_library(LZO_LIB lzo2)
endif()
if(WIN32 OR LZO_LIB STREQUAL LZO_LIB-NOTFOUND)
include_directories(include)
message("-- Using Athena's built-in lzo")
add_library(lzo2
    src/lzo1.c
    src/lzo1_99.c
    src/lzo1a.c
    src/lzo1a_99.c
    src/lzo1b_1.c
    src/lzo1b_2.c
    src/lzo1b_3.c
    src/lzo1b_4.c
    src/lzo1b_5.c
    src/lzo1b_6.c
    src/lzo1b_7.c
    src/lzo1b_8.c
    src/lzo1b_9.c
    src/lzo1b_99.c
    src/lzo1b_9x.c
    src/lzo1b_cc.c
    src/lzo1b_d1.c
    src/lzo1b_d2.c
    src/lzo1b_rr.c
    src/lzo1b_xx.c
    src/lzo1c_1.c
    src/lzo1c_2.c
    src/lzo1c_3.c
    src/lzo1c_4.c
    src/lzo1c_5.c
    src/lzo1c_6.c
    src/lzo1c_7.c
    src/lzo1c_8.c
    src/lzo1c_9.c
    src/lzo1c_99.c
    src/lzo1c_9x.c
    src/lzo1c_cc.c
    src/lzo1c_d1.c
    src/lzo1c_d2.c
    src/lzo1c_rr.c
    src/lzo1c_xx.c
    src/lzo1f_1.c
    src/lzo1f_9x.c
    src/lzo1f_d1.c
    src/lzo1f_d2.c
    src/lzo1x_1.c
    src/lzo1x_1k.c
    src/lzo1x_1l.c
    src/lzo1x_1o.c
    src/lzo1x_9x.c
    src/lzo1x_d1.c
    src/lzo1x_d2.c
    src/lzo1x_d3.c
    src/lzo1x_o.c
    src/lzo1y_1.c
    src/lzo1y_9x.c
    src/lzo1y_d1.c
    src/lzo1y_d2.c
    src/lzo1y_d3.c
    src/lzo1y_o.c
    src/lzo1z_9x.c
    src/lzo1z_d1.c
    src/lzo1z_d2.c
    src/lzo1z_d3.c
    src/lzo2a_9x.c
    src/lzo2a_d1.c
    src/lzo2a_d2.c
    src/lzo_crc.c
    src/lzo_init.c
    src/lzo_ptr.c
    src/lzo_str.c
    src/lzo_util.c
    include/lzo/lzo1.h
    include/lzo/lzo1x.h
    include/lzo/lzoconf.h
    include/lzo/lzodefs.h
    include/lzo/lzoutil.h)
if(WIN32 AND NOT UNIX)
install(DIRECTORY include/lzo DESTINATION include COMPONENT lzo2)
install(TARGETS lzo2 DESTINATION lib COMPONENT lzo2)
endif()
set(LZO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "LZO include path" FORCE)
endif()
