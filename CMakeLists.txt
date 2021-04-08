cmake_minimum_required(VERSION 3.10 FATAL_ERROR) # because of c++17
# Set MSVC runtime library flags from CMAKE_MSVC_RUNTIME_LIBRARY
cmake_policy(SET CMP0091 NEW)
project(athena)
if (NOT MSVC)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

##################
# Athena Version #
##################

set(ATHENA_MAJOR_VERSION 2)
set(ATHENA_MINOR_VERSION 3)
set(ATHENA_PATCH_VERSION 0)
set(ATHENA_VERSION
  ${ATHENA_MAJOR_VERSION}.${ATHENA_MINOR_VERSION}.${ATHENA_PATCH_VERSION})

################
# Athena Build #
################

add_subdirectory(extern)

add_library(athena-core
    src/athena/Utility.cpp
    src/athena/MemoryReader.cpp
    src/athena/MemoryWriter.cpp
    src/athena/VectorWriter.cpp
    src/athena/FileWriterGeneric.cpp
    src/athena/Global.cpp
    src/athena/Checksums.cpp
    src/athena/Compression.cpp
    src/athena/Socket.cpp
    src/LZ77/LZLookupTable.cpp
    src/LZ77/LZType10.cpp
    src/LZ77/LZType11.cpp
    src/LZ77/LZBase.cpp
    src/athena/FileInfo.cpp
    src/athena/Dir.cpp
    src/athena/DNAYaml.cpp

    include/athena/IStream.hpp
    include/athena/IStreamReader.hpp
    include/athena/IStreamWriter.hpp
    include/athena/Types.hpp
    include/athena/Utility.hpp
    include/athena/Global.hpp
    include/athena/FileReader.hpp
    include/athena/FileWriter.hpp
    include/athena/MemoryReader.hpp
    include/athena/MemoryWriter.hpp
    include/athena/VectorWriter.hpp
    include/athena/Checksums.hpp
    include/athena/ChecksumsLiterals.hpp
    include/athena/Compression.hpp
    include/athena/Socket.hpp
    include/LZ77/LZBase.hpp
    include/LZ77/LZLookupTable.hpp
    include/LZ77/LZType10.hpp
    include/LZ77/LZType11.hpp
    include/athena/FileInfo.hpp
    include/athena/Dir.hpp
    include/athena/DNA.hpp
    include/athena/DNAYaml.hpp
    include/athena/DNAOp.hpp
    include/athena/YAMLCommon.hpp
    include/athena/YAMLDocReader.hpp
    include/athena/YAMLDocWriter.hpp
    include/yaml.h
    include/utf8proc.h
)
if(WIN32)
    target_sources(athena-core PRIVATE
        src/win32_largefilewrapper.c
        include/win32_largefilewrapper.h
        src/athena/FileWriterWin32.cpp
        src/athena/FileReaderWin32.cpp
    )

    target_compile_definitions(athena-core PRIVATE
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
    )
else()
    target_sources(athena-core PRIVATE
        src/athena/FileWriterNix.cpp
        src/athena/FileReader.cpp
    )
    if(APPLE OR GEKKO OR NX OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
        target_sources(athena-core PRIVATE
            src/osx_largefilewrapper.c
            include/osx_largefilewrapper.h
        )

        if(GEKKO OR NX)
            target_sources(athena-core PRIVATE
                src/gekko_support.c
                include/gekko_support.h
            )
        endif()
    endif()
endif()

target_include_directories(athena-core PUBLIC
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${ZLIB_INCLUDE_DIR}>
)
target_link_libraries(athena-core PUBLIC
    athena-libyaml
    fmt
)

add_library(athena-sakura EXCLUDE_FROM_ALL
    src/athena/Sprite.cpp
    src/athena/SpriteFile.cpp
    src/athena/SpriteFileReader.cpp
    src/athena/SpriteFileWriter.cpp
    src/athena/SpriteFrame.cpp
    src/athena/SpritePart.cpp

    include/athena/SakuraGlobal.hpp
    include/athena/Sprite.hpp
    include/athena/SpriteFile.hpp
    include/athena/SpriteFileReader.hpp
    include/athena/SpriteFileWriter.hpp
    include/athena/SpriteFrame.hpp
    include/athena/SpritePart.hpp
)

add_library(athena-wiisave EXCLUDE_FROM_ALL
    src/athena/WiiBanner.cpp
    src/athena/WiiFile.cpp
    src/athena/WiiImage.cpp
    src/athena/WiiSave.cpp
    src/athena/WiiSaveReader.cpp
    src/athena/WiiSaveWriter.cpp
    src/bn.cpp
    src/ec.cpp
    src/md5.cpp
    src/sha1.cpp
    src/aes.cpp

    include/athena/WiiBanner.hpp
    include/athena/WiiFile.hpp
    include/athena/WiiImage.hpp
    include/athena/WiiSave.hpp
    include/athena/WiiSaveReader.hpp
    include/athena/WiiSaveWriter.hpp
    include/aes.hpp
    include/bn.hpp
    include/ec.hpp
    include/md5.h
    include/sha1.h
)
if(NOT MSVC AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL x86_64)
    set_source_files_properties(src/aes.cpp PROPERTIES COMPILE_FLAGS -maes)
endif()


add_library(athena-zelda EXCLUDE_FROM_ALL
    src/athena/ALTTPFile.cpp
    src/athena/ALTTPFileReader.cpp
    src/athena/ALTTPFileWriter.cpp
    src/athena/ALTTPQuest.cpp
    src/athena/MCFile.cpp
    src/athena/MCFileReader.cpp
    src/athena/MCFileWriter.cpp
    src/athena/MCSlot.cpp
    src/athena/SkywardSwordFile.cpp
    src/athena/SkywardSwordFileReader.cpp
    src/athena/SkywardSwordFileWriter.cpp
    src/athena/SkywardSwordQuest.cpp
    src/athena/ZQuestFile.cpp
    src/athena/ZQuestFileReader.cpp
    src/athena/ZQuestFileWriter.cpp

    include/athena/ALTTPEnums.hpp
    include/athena/ALTTPFile.hpp
    include/athena/ALTTPFileReader.hpp
    include/athena/ALTTPFileWriter.hpp
    include/athena/ALTTPQuest.hpp
    include/athena/ALTTPStructs.hpp
    include/athena/MCFile.hpp
    include/athena/MCFileReader.hpp
    include/athena/MCFileWriter.hpp
    include/athena/SkywardSwordFile.hpp
    include/athena/SkywardSwordFileReader.hpp
    include/athena/SkywardSwordFileWriter.hpp
    include/athena/SkywardSwordQuest.hpp
    include/athena/ZQuestFile.hpp
    include/athena/ZQuestFileReader.hpp
    include/athena/ZQuestFileWriter.hpp
)

# Icon
set(ATHENA_ICO ${CMAKE_CURRENT_SOURCE_DIR}/Athena.ico)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(INSTALL_CMAKE_DIR cmake)
else()
  set(INSTALL_CMAKE_DIR lib/cmake/athena)
endif()

# Make relative paths absolute (needed later on)
foreach(p LIB INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(ABS_${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  else()
    set(ABS_${var} "${${var}}")
  endif()
endforeach()

# Define installs
install(DIRECTORY include DESTINATION ${INSTALL_INCLUDE_DIR}/athena COMPONENT athena)
install(DIRECTORY extern/fmt/include DESTINATION ${INSTALL_INCLUDE_DIR}/fmt COMPONENT athena)
install(TARGETS athena-core fmt
        DESTINATION ${INSTALL_LIB_DIR} EXPORT AthenaTargets COMPONENT athena)
if(WIN32 AND NOT CYGWIN)
install(FILES Athena.ico DESTINATION ${INSTALL_LIB_DIR} COMPONENT athena)
elseif(UNIX)
configure_file(libAthena.pc.in libAthena.pc @ONLY)
configure_file(PKGBUILD.in ${CMAKE_CURRENT_SOURCE_DIR}/PKGBUILD @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libAthena.pc
        DESTINATION ${INSTALL_LIB_DIR}/pkgconfig COMPONENT athena)
endif()

##################
# Package Export #
##################

# Add all targets to the build-tree export set
export(TARGETS athena-core athena-libyaml fmt FILE "${CMAKE_CURRENT_BINARY_DIR}/AthenaTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE athena)

# Create the AthenaConfig.cmake
# ... for the build tree
set(CONF_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/athena")
set(CONF_ICON_DIR "${PROJECT_SOURCE_DIR}/Athena.ico")
configure_file(AthenaConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/AthenaConfig.cmake" @ONLY)
# ... for the install tree
file(RELATIVE_PATH REL_INCLUDE_DIR "${ABS_INSTALL_CMAKE_DIR}" "${ABS_INSTALL_INCLUDE_DIR}")
file(RELATIVE_PATH REL_ICON_DIR "${ABS_INSTALL_CMAKE_DIR}" "${ABS_INSTALL_LIB_DIR}/Athena.ico")
set(CONF_INCLUDE_DIR "\${ATHENA_CMAKE_DIR}/${REL_INCLUDE_DIR}/athena")
set(CONF_ICON_DIR "\${ATHENA_CMAKE_DIR}/${REL_ICON_DIR}")
configure_file(AthenaConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/AthenaConfig.cmake" @ONLY)
# ... for both
configure_file(AthenaConfigVersion.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/AthenaConfigVersion.cmake" @ONLY)

# Install AthenaConfig.cmake
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/AthenaConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/AthenaConfigVersion.cmake"
  DESTINATION ${INSTALL_CMAKE_DIR} COMPONENT athena)

# Install the export set for use with the install-tree
install(EXPORT AthenaTargets DESTINATION ${INSTALL_CMAKE_DIR} COMPONENT athena)

################
# atdna import #
################

add_subdirectory(atdna)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
# Test target
add_executable(atdna-test atdna/test.cpp atdna/test.hpp)
if (CMAKE_SYSTEM_NAME STREQUAL "Switch")
    set_target_properties(atdna-test PROPERTIES SUFFIX ".elf")
    target_link_libraries(atdna-test athena-core nx)
else()
    target_link_libraries(atdna-test athena-core)
endif()
target_atdna(atdna-test atdna_test.cpp atdna/test.hpp)
endif()

#########
# CPack #
#########

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Athena")
set(CPACK_PACKAGE_VENDOR "Antidote / Jackoalan")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR ${ATHENA_MAJOR_VERSION})
set(CPACK_PACKAGE_VERSION_MINOR ${ATHENA_MINOR_VERSION})
set(CPACK_PACKAGE_VERSION_PATCH ${ATHENA_PATCH_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "athena")
if(WIN32 AND NOT UNIX)
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "athena")
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\AthenaNSIS.bmp")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\Athena.ico")
    set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}\\\\Athena.ico")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "Uninstall.exe")
    set(CPACK_NSIS_DISPLAY_NAME "Athena")
    set(CPACK_NSIS_URL_INFO_ABOUT "http://libathena.github.io")
    set(CPACK_NSIS_CONTACT "antidote.crk@gmail.com")
endif()
include(CPack)

