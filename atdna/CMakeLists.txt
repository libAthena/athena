###############
# ATDNA Build #
###############

if(NOT CMAKE_CROSSCOMPILING)

string(REPLACE -stdlib=libc++ "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

get_directory_property(ATDNA_DEFINES COMPILE_DEFINITIONS)
list(REMOVE_ITEM ATDNA_DEFINES _GLIBCXX_DEBUG=1)
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "${ATDNA_DEFINES}")

# Force this binary to Release flags (to make MSVC happy)
if(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE}")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /O2 /Ob2 /DNDEBUG")
  set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "-debug -incremental:no")
endif()

# Find dependencies
include(FindLLVM.cmake)
if(NOT LLVM_FOUND)
    message(STATUS "Unable to locate LLVM installation; skipping atdna")
else()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  option(ATDNA_DYNAMIC_LLVM "Use dynamic library targets when linking LLVM" ON)
else()
  option(ATDNA_DYNAMIC_LLVM "Use dynamic library targets when linking LLVM" OFF)
endif()

if(ATDNA_DYNAMIC_LLVM)
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
       LLVM)
else()
  find_library(LLVMDEMANGLE_LIB LLVMDemangle HINTS "${LLVM_ROOT_DIR}/lib")
  find_library(LLVMBINARYFORMAT_LIB LLVMBinaryFormat HINTS "${LLVM_ROOT_DIR}/lib")
  if (NOT LLVMDEMANGLE_LIB)
    set(LLVMDEMANGLE_LIB "")
  endif()
  if (NOT LLVMBINARYFORMAT_LIB)
    set(LLVMBINARYFORMAT_LIB "")
  endif()
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
       LLVMCore
       LLVMOption
       LLVMMCParser
       LLVMBitReader
       ${LLVMBINARYFORMAT_LIB}
       LLVMMC
       LLVMProfileData
       LLVMSupport
       ${LLVMDEMANGLE_LIB})
endif()

string(FIND ${LLVM_VERSION_STRING} "svn" SVN_FILTER_IDX)
if(NOT SVN_FILTER_IDX EQUAL -1)
   string(SUBSTRING ${LLVM_VERSION_STRING} 0 ${SVN_FILTER_IDX} LLVM_VERSION_BASE)
else()
   set(LLVM_VERSION_BASE ${LLVM_VERSION_STRING})
endif()

set(CLANG_INCLUDE_DIR ${LLVM_LIBRARY_DIRS}/clang/${LLVM_VERSION_BASE}/include
    CACHE PATH "Clang include dir" FORCE)

if(UNIX)
  list(APPEND PLAT_LIBS z pthread curses)
  if (APPLE)
    list(APPEND PLAT_LIBS dl)
  elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    list(APPEND PLAT_LIBS dl tinfo)
  endif()
elseif(WIN32)
  list(APPEND PLAT_LIBS Version)
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
add_executable(atdna main.cpp test.hpp ${PLAT_SRCS})
target_link_libraries(atdna ${LLVM_LIBS} ${PLAT_LIBS})
set_source_files_properties(main.cpp PROPERTIES COMPILE_DEFINITIONS
                            "INSTALL_PREFIX=${ABS_INSTALL_BIN_DIR};__STDC_LIMIT_MACROS=1;__STDC_CONSTANT_MACROS=1")
target_include_directories(atdna PRIVATE ${LLVM_INCLUDE_DIRS})
target_link_directories(atdna PRIVATE ${LLVM_LIBRARY_DIRS})
if(MSVC)
  target_compile_options(atdna PRIVATE /GR-)
else()
  target_compile_options(atdna PRIVATE -fno-rtti -Wno-error)
endif()

# Define installs
install(TARGETS atdna DESTINATION ${INSTALL_BIN_DIR} EXPORT atdnaTargets COMPONENT atdna)
install(DIRECTORY ${CLANG_INCLUDE_DIR}/ DESTINATION ${INSTALL_INCLUDE_DIR}/athena/clang COMPONENT atdna)

##################
# Package Export #
##################

# Add all targets to the build-tree export set
export(TARGETS atdna FILE "${CMAKE_CURRENT_BINARY_DIR}/atdnaTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE atdna)

# Create the atdnaConfig.cmake
# ... for the build tree
set(CONF_CLANG_INCLUDE_DIR "${CLANG_INCLUDE_DIR}")
configure_file(atdnaConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/atdnaConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_CLANG_INCLUDE_DIR "\${ATHENA_INCLUDE_DIR}/clang")
configure_file(atdnaConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atdnaConfig.cmake" @ONLY)
# ... for both
configure_file(atdnaConfigVersion.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/atdnaConfigVersion.cmake" @ONLY)

# Install atdnaConfig.cmake
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/atdnaConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/atdnaConfigVersion.cmake"
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

include(atdnaHelpers.cmake)

endif()
