#
# gather_include_directories recursively builds a list of include directories
# across all dependencies.
#

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.20")
  # Don't transform add_custom_command DEPFILE for Ninja
  cmake_policy(SET CMP0116 OLD)
endif ()

function(_atdna_gather_include_directories_impl target_name)
  get_target_property(target_dependencies ${target_name} INTERFACE_LINK_LIBRARIES)
  foreach(dep ${target_dependencies})
    if(TARGET ${dep})
      get_target_property(dep_includes ${dep} INTERFACE_INCLUDE_DIRECTORIES)
      if(dep_includes)
        list(APPEND target_includes ${dep_includes})
      endif()
      _atdna_gather_include_directories_impl(${dep})
    endif()
  endforeach()
  set(target_includes ${target_includes} PARENT_SCOPE)
endfunction()

function(atdna_gather_include_directories var target_name)
  unset(target_includes)
  get_directory_property(dir_includes INCLUDE_DIRECTORIES)
  if(dir_includes)
    list(APPEND target_includes ${dir_includes})
  endif()
  get_target_property(target_includes1 ${target_name} INCLUDE_DIRECTORIES)
  if(target_includes1)
    list(APPEND target_includes ${target_includes1})
  endif()
  get_target_property(target_includes2 ${target_name} INTERFACE_INCLUDE_DIRECTORIES)
  if(target_includes2)
    list(APPEND target_includes ${target_includes2})
  endif()
  _atdna_gather_include_directories_impl(${target_name})
  list(REMOVE_DUPLICATES target_includes)
  set(${var} ${target_includes} PARENT_SCOPE)
endfunction()

function(_atdna_gather_compile_definitions_impl target_name)
  get_target_property(target_dependencies ${target_name} INTERFACE_LINK_LIBRARIES)
  foreach(dep ${target_dependencies})
    if(TARGET ${dep})
      get_target_property(dep_defines ${dep} INTERFACE_COMPILE_DEFINITIONS)
      if(dep_defines)
        list(APPEND target_defines ${dep_defines})
      endif()
      _atdna_gather_compile_definitions_impl(${dep})
    endif()
  endforeach()
  set(target_defines ${target_defines} PARENT_SCOPE)
endfunction()

function(atdna_gather_compile_definitions var target_name)
  unset(target_defines)
  get_directory_property(dir_defines COMPILE_DEFINITIONS)
  if(dir_defines)
    list(APPEND target_defines ${dir_defines})
  endif()
  get_target_property(target_defines1 ${target_name} COMPILE_DEFINITIONS)
  if(target_defines1)
    list(APPEND target_defines ${target_defines1})
  endif()
  get_target_property(target_defines2 ${target_name} INTERFACE_COMPILE_DEFINITIONS)
  if(target_defines2)
    list(APPEND target_defines ${target_defines2})
  endif()
  _atdna_gather_compile_definitions_impl(${target_name})
  list(REMOVE_DUPLICATES target_defines)
  set(${var} ${target_defines} PARENT_SCOPE)
endfunction()

#######################
# In-tree atdna macro #
#######################

# Super handy macro for adding atdna target
function(atdna out incdirs cdefs)
  # Ninja wants invocations in root binary dir for DEPFILE application
  file(RELATIVE_PATH out_rel ${CMAKE_BINARY_DIR} "${CMAKE_CURRENT_BINARY_DIR}/${out}")

  # Make input files source-relative
  unset(ins)
  unset(ins_impdeps)
  foreach(arg ${ARGN})
    list(APPEND ins ${CMAKE_CURRENT_SOURCE_DIR}/${arg})
    list(APPEND ins_impdeps CXX)
    list(APPEND ins_impdeps ${CMAKE_CURRENT_SOURCE_DIR}/${arg})
  endforeach()

  set(inccli "")
  foreach(dir ${incdirs})
    if (NOT dir MATCHES ".*emscripten.*")
      list(APPEND inccli "-I${dir}")
    endif()
  endforeach()

  set(cdefcli "")
  foreach(def ${cdefs})
    list(APPEND cdefcli "-D${def}")
  endforeach()

  # MS extra
  unset(extraargs)
  if(MSVC)
    list(APPEND extraargs -fms-compatibility -fexceptions)
    if(MSVC_VERSION EQUAL 1800)
      list(APPEND extraargs -fms-compatibility-version=18.00)
    elseif(MSVC_VERSION EQUAL 1900)
      list(APPEND extraargs -fms-compatibility-version=19.00)
    elseif(MSVC_VERSION EQUAL 1910)
      list(APPEND extraargs -fms-compatibility-version=19.10)
    elseif(MSVC_VERSION EQUAL 1911)
      list(APPEND extraargs -fms-compatibility-version=19.11)
  endif()

  # OSX Extra
  elseif(APPLE)
    get_filename_component(COMPILER_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
    if (NOT EXISTS "${CMAKE_OSX_SYSROOT}")
      message(FATAL_ERROR "CMAKE_OSX_SYSROOT not set")
    endif()
    file(GLOB SYSTEM_INCLUDE_DIR "${COMPILER_DIR}/../lib/clang/*/include")
    list(APPEND extraargs
         -isysroot ${CMAKE_OSX_SYSROOT}
         -stdlib++-isystem "${COMPILER_DIR}/../include/c++/v1"
         -isystem "${SYSTEM_INCLUDE_DIR}")
  endif()

  # Make target
  if(${CMAKE_GENERATOR} STREQUAL "Ninja")
    # Use Ninja's DEPFILE parser in cooperation with atdna
    add_custom_command(OUTPUT ${out} COMMAND $<TARGET_FILE:atdna>
                       ARGS ${extraargs} -o ${out_rel} -MD -MT ${out_rel} -MF ${out_rel}.d
                       ${inccli} ${cdefcli}
                       "-I${athena_SOURCE_DIR}/include" ${ins}
                       DEPENDS atdna ${ins} IMPLICIT_DEPENDS ${ins_impdeps}
                       DEPFILE "${CMAKE_CURRENT_BINARY_DIR}/${out}.d"
                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                       COMMENT "Generating DNA ${out_rel}"
                       COMMAND_EXPAND_LISTS)
  else()
    # Use CMake's built-in dependency scanner for makefile targets
    add_custom_command(OUTPUT ${out} COMMAND $<TARGET_FILE:atdna>
                       ARGS ${extraargs} -o ${out_rel}
                       ${inccli} ${cdefcli}
                       "-I${athena_SOURCE_DIR}/include" ${ins}
                       DEPENDS atdna ${ins} IMPLICIT_DEPENDS ${ins_impdeps}
                       WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                       COMMENT "Generating DNA ${out_rel}"
                       COMMAND_EXPAND_LISTS)
  endif()
endfunction()

function(target_atdna target out)
  atdna_gather_include_directories(include_list ${target})
  atdna_gather_compile_definitions(define_list ${target})
  atdna(${out} "${include_list}" "${define_list}" ${ARGN})
  target_sources(${target} PRIVATE ${out} ${ARGN})
endfunction()
