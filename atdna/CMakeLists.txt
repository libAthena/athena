###############
# ATDNA Build #
###############

# Find dependencies
include(FindLLVM.cmake)
if(NOT LLVM_FOUND)
    message("-- Unable to locate LLVM installation; skipping atdna")
else()
list(APPEND LLVM_LIBS
     clangFrontend
     clangTooling
     clangDriver
     clangSerialization
     clangParse
     clangSema
     clangAnalysis
     clangEdit
     clangAST
     clangLex
     clangBasic
     LLVMOption
     LLVMMCParser
     LLVMBitReader
     LLVMMC
     LLVMSupport)
set(CLANG_INCLUDE_DIR ${LLVM_LIBRARY_DIRS}/clang/${LLVM_VERSION_STRING}/include)

if(UNIX)
list(APPEND PLAT_LIBS z pthread curses dl)
endif()

# Offer the user the choice of overriding the installation directories
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
if(WIN32 AND NOT CYGWIN)
  set(INSTALL_CMAKE_DIR cmake)
else()
  set(INSTALL_CMAKE_DIR lib/cmake/atdna)
endif()

# Make relative paths absolute (needed later on)
foreach(p BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(ABS_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  else()
    set(ABS_${var} "${${var}}")
  endif()
endforeach()

# Windows resource
if(WIN32)
  configure_file(main.rc.in main.rc @ONLY)
  set(PLAT_SRCS ${CMAKE_CURRENT_BINARY_DIR}/main.rc)
endif()

# ATDNA target
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_executable(atdna main.cpp test.hpp ${PLAT_SRCS})
target_link_libraries(atdna ${LLVM_LIBS} ${PLAT_LIBS})
set_source_files_properties(main.cpp PROPERTIES COMPILE_DEFINITIONS 
                            "INSTALL_PREFIX=${ABS_INSTALL_BIN_DIR};__STDC_LIMIT_MACROS=1;__STDC_CONSTANT_MACROS=1")
if(MSVC)
set_target_properties(atdna PROPERTIES
                      COMPILE_FLAGS "/GR-")
else()
set_target_properties(atdna PROPERTIES
                      COMPILE_FLAGS "-std=c++11 -fno-rtti -Wno-unused-parameter"
                      LINK_FLAGS -std=c++11)
endif()

# Define installs
install(TARGETS atdna DESTINATION ${INSTALL_BIN_DIR} EXPORT atdnaTargets COMPONENT atdna)
install(DIRECTORY ${CLANG_INCLUDE_DIR}/ DESTINATION ${INSTALL_INCLUDE_DIR}/Athena/clang COMPONENT atdna)

##################
# Package Export #
##################

# Add all targets to the build-tree export set
export(TARGETS atdna FILE "${PROJECT_BINARY_DIR}/atdnaTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE atdna)

# Create the atdnaConfig.cmake
# ... for the build tree
set(CONF_CLANG_INCLUDE_DIR "${CLANG_INCLUDE_DIR}")
configure_file(atdnaConfig.cmake.in "${PROJECT_BINARY_DIR}/atdnaConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_CLANG_INCLUDE_DIR "\${ATHENA_INCLUDE_DIR}/clang")
configure_file(atdnaConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atdnaConfig.cmake" @ONLY)
# ... for both
configure_file(atdnaConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/atdnaConfigVersion.cmake" @ONLY)

# Install atdnaConfig.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atdnaConfig.cmake"
  "${PROJECT_BINARY_DIR}/atdnaConfigVersion.cmake"
  DESTINATION ${INSTALL_CMAKE_DIR} COMPONENT atdna)

# Install the export set for use with the install-tree
install(EXPORT atdnaTargets DESTINATION ${INSTALL_CMAKE_DIR} COMPONENT atdna)

#########
# CTest #
#########

enable_testing()
add_test(NAME test-dna COMMAND $<TARGET_FILE:atdna> -o test.cpp 
         "-I${ATHENA_INCLUDE_DIR}" -isystem "${CLANG_INCLUDE_DIR}"
         ${CMAKE_SOURCE_DIR}/test.hpp)

endif()

