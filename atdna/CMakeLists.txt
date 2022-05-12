###############
# ATDNA Build #
###############

if(NOT TARGET atdna AND NOT CMAKE_CROSSCOMPILING)

get_directory_property(ATDNA_DEFINES COMPILE_DEFINITIONS)
list(REMOVE_ITEM ATDNA_DEFINES _GLIBCXX_DEBUG=1)
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS "${ATDNA_DEFINES}")

# Find dependencies
if (NOT "${LLVM_ROOT_DIR}" STREQUAL "")
  # use existing LLVM_ROOT_DIR
elseif (APPLE AND CMAKE_SYSTEM_PROCESSOR STREQUAL x86_64)
  set(LLVM_ROOT_DIR /usr/local/opt/llvm)
elseif (APPLE AND CMAKE_SYSTEM_PROCESSOR STREQUAL arm64)
  set(LLVM_ROOT_DIR /opt/homebrew/opt/llvm)
elseif (WIN32)
  get_filename_component(LLVM_ROOT_DIR [HKEY_LOCAL_MACHINE\\Software\\LLVM\\LLVM] ABSOLUTE)
else ()
  set(LLVM_ROOT_DIR "")
endif ()
find_package(Clang REQUIRED PATHS ${LLVM_ROOT_DIR})
find_package(LLVM REQUIRED PATHS ${CLANG_INSTALL_PREFIX})

if(LLVM_FOUND)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  option(ATDNA_DYNAMIC_LLVM "Use dynamic library targets when linking LLVM" ON)
else()
  option(ATDNA_DYNAMIC_LLVM "Use dynamic library targets when linking LLVM" OFF)
endif()

set(CLANG_LIBS "")
set(LLVM_LIBS "")

if (ATDNA_DYNAMIC_LLVM)
  list(APPEND CLANG_LIBS clang-cpp)
  list(APPEND LLVM_LIBS LLVM)
else()
  function(recursive_remove_library target library)
    get_target_property(target_dependencies ${target} INTERFACE_LINK_LIBRARIES)
    foreach(dep ${target_dependencies})
      if (NOT "${dep}" STREQUAL "${library}" AND TARGET "${dep}")
        recursive_remove_library(${dep} ${library})
      endif()
    endforeach()
    list(FIND target_dependencies "${library}" list_index)
    if (${list_index} GREATER -1)
      list(REMOVE_AT target_dependencies ${list_index})
      set_property(TARGET ${target} PROPERTY INTERFACE_LINK_LIBRARIES "${target_dependencies}")
    endif()
  endfunction()

  # Add LLVM static libs
  list(APPEND CLANG_LIBS clangTooling)
  list(APPEND LLVM_LIBS LLVMFrontendOpenMP LLVMOption)
  # Remove LLVM dynamic library from Clang dependencies
  foreach (lib ${CLANG_LIBS})
    recursive_remove_library(${lib} LLVM)
  endforeach ()
  # Hack around link order issues
  target_link_libraries(clangAST INTERFACE LLVMFrontendOpenMP)
endif()

# Offer the user the choice of overriding the installation directories
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_CMAKE_DIR lib/cmake/atdna)

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
target_link_libraries(atdna ${CLANG_LIBS} ${LLVM_LIBS})
if (APPLE)
  set(ATDNA_ARGV0 clang-tool)
else ()
  set(ATDNA_ARGV0 "${LLVM_TOOLS_BINARY_DIR}/clang-tool")
endif ()
target_compile_definitions(atdna PRIVATE
        INSTALL_PREFIX=${ABS_INSTALL_BIN_DIR}
        __STDC_LIMIT_MACROS=1
        __STDC_CONSTANT_MACROS=1
        ATDNA_ARGV0=${ATDNA_ARGV0})
target_include_directories(atdna PRIVATE ${CLANG_INCLUDE_DIRS})
# Clang 10.0.x headers currently broken with C++20
set_property(TARGET atdna PROPERTY CXX_STANDARD 17)
if(MSVC)
  if (VCPKG_TOOLCHAIN)
    # Assumes static build of LLVM
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      set_property(TARGET atdna PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    else ()
      set_property(TARGET atdna PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
    endif ()
  else ()
    # Allow linking against release-built LLVM libraries
    target_compile_options(atdna PRIVATE /GR- /D_ITERATOR_DEBUG_LEVEL=0)
    set_property(TARGET atdna PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
  endif ()
else ()
  target_compile_options(atdna PRIVATE -fno-rtti -fvisibility=hidden -Wno-error)
  if (METAFORCE_ASAN AND CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # On Linux, we need to compile without ASAN; explicitly disable it here
    target_compile_options(atdna PRIVATE -stdlib=libstdc++ -fno-sanitize=address)
    target_link_options(atdna PRIVATE -stdlib=libstdc++ -fno-sanitize=address)
  endif ()
endif()

# Define installs
install(TARGETS atdna DESTINATION ${INSTALL_BIN_DIR} EXPORT atdnaTargets COMPONENT atdna)
install(DIRECTORY ${LLVM_INCLUDE_DIR}/clang DESTINATION ${INSTALL_INCLUDE_DIR}/athena COMPONENT atdna)

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
configure_file(atdnaConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/atdnaConfig.cmake" @ONLY)
# ... for the install tree
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

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
enable_testing()
add_test(NAME test-dna COMMAND $<TARGET_FILE:atdna> -o test.cpp
         "-I${ATHENA_INCLUDE_DIR}" ${CMAKE_SOURCE_DIR}/test.hpp)
endif()

endif()

endif()

include(atdnaHelpers.cmake)
