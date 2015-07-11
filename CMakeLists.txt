cmake_minimum_required(VERSION 3.0)
project(Athena)

##################
# Athena Version #
##################

set(ATHENA_MAJOR_VERSION 2)
set(ATHENA_MINOR_VERSION 0)
set(ATHENA_PATCH_VERSION 0)
set(ATHENA_VERSION
  ${ATHENA_MAJOR_VERSION}.${ATHENA_MINOR_VERSION}.${ATHENA_PATCH_VERSION})

################
# Athena Build #
################

add_subdirectory(extern)

include_directories(include ${LZO_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(WIN32)
list(APPEND CORE_EXTRA src/win32_largefilewrapper.c)
elseif(APPLE)
list(APPEND CORE_EXTRA src/osx_largefilewrapper.c)
elseif(GEKKO)
list(APPEND CORE_EXTRA src/gekko_support.c)
endif()

add_library(AthenaCore
    src/Athena/Utility.cpp
    src/Athena/FileReader.cpp
    src/Athena/FileWriter.cpp
    src/Athena/MemoryReader.cpp
    src/Athena/MemoryWriter.cpp
    src/Athena/Global.cpp
    src/Athena/Checksums.cpp
    src/Athena/Compression.cpp
    src/LZ77/LZLookupTable.cpp
    src/LZ77/LZType10.cpp
    src/LZ77/LZType11.cpp
    src/LZ77/LZBase.cpp
    src/Athena/FileInfo.cpp
    src/Athena/Dir.cpp
    ${CORE_EXTRA}

    include/Athena/IStream.hpp
    include/Athena/IStreamReader.hpp
    include/Athena/IStreamWriter.hpp
    include/Athena/Types.hpp
    include/Athena/Utility.hpp
    include/Athena/Global.hpp
    include/Athena/Exception.hpp
    include/Athena/FileNotFoundException.hpp
    include/Athena/IOException.hpp
    include/Athena/InvalidDataException.hpp
    include/Athena/InvalidOperationException.hpp
    include/Athena/FileReader.hpp
    include/Athena/FileWriter.hpp
    include/Athena/MemoryReader.hpp
    include/Athena/MemoryWriter.hpp
    include/Athena/NotImplementedException.hpp
    include/Athena/Checksums.hpp
    include/Athena/Compression.hpp
    include/LZ77/LZBase.hpp
    include/LZ77/LZLookupTable.hpp
    include/LZ77/LZType10.hpp
    include/LZ77/LZType11.hpp
    include/Athena/FileInfo.hpp
    include/Athena/Dir.hpp
    include/gekko_support.h
    include/Athena/DNA.hpp
)

add_library(AthenaSakura
    src/Athena/Sprite.cpp
    src/Athena/SpriteFile.cpp
    src/Athena/SpriteFileReader.cpp
    src/Athena/SpriteFileWriter.cpp
    src/Athena/SpriteFrame.cpp
    src/Athena/SpritePart.cpp

    include/Athena/Sprite.hpp
    include/Athena/SpriteFile.hpp
    include/Athena/SpriteFileReader.hpp
    include/Athena/SpriteFileWriter.hpp
    include/Athena/SpriteFrame.hpp
    include/Athena/SpritePart.hpp
)

add_library(AthenaWiiSave
    src/Athena/WiiBanner.cpp
    src/Athena/WiiFile.cpp
    src/Athena/WiiImage.cpp
    src/Athena/WiiSave.cpp
    src/Athena/WiiSaveReader.cpp
    src/Athena/WiiSaveWriter.cpp
    src/bn.cpp
    src/ec.cpp
    src/md5.cpp
    src/sha1.cpp
    src/aes.cpp

    include/Athena/WiiBanner.hpp
    include/Athena/WiiFile.hpp
    include/Athena/WiiImage.hpp
    include/Athena/WiiSave.hpp
    include/Athena/WiiSaveReader.hpp
    include/Athena/WiiSaveWriter.hpp
    include/aes.hpp
    include/bn.h
    include/ec.h
    include/md5.h
    include/sha1.h
)
set_source_files_properties(src/aes.cpp PROPERTIES COMPILE_FLAGS -maes)

add_library(AthenaZelda
    src/Athena/ALTTPFile.cpp
    src/Athena/ALTTPFileReader.cpp
    src/Athena/ALTTPFileWriter.cpp
    src/Athena/ALTTPQuest.cpp
    src/Athena/MCFile.cpp
    src/Athena/MCFileReader.cpp
    src/Athena/MCFileWriter.cpp
    src/Athena/MCSlot.cpp
    src/Athena/SkywardSwordFile.cpp
    src/Athena/SkywardSwordFileReader.cpp
    src/Athena/SkywardSwordFileWriter.cpp
    src/Athena/SkywardSwordQuest.cpp
    src/Athena/ZQuestFile.cpp
    src/Athena/ZQuestFileReader.cpp
    src/Athena/ZQuestFileWriter.cpp

    include/Athena/ALTTPEnums.hpp
    include/Athena/ALTTPFile.hpp
    include/Athena/ALTTPFileReader.hpp
    include/Athena/ALTTPFileWriter.hpp
    include/Athena/ALTTPQuest.hpp
    include/Athena/ALTTPStructs.hpp
    include/Athena/MCFile.hpp
    include/Athena/MCFileReader.hpp
    include/Athena/MCFileWriter.hpp
    include/Athena/SkywardSwordFile.hpp
    include/Athena/SkywardSwordFileReader.hpp
    include/Athena/SkywardSwordFileWriter.hpp
    include/Athena/SkywardSwordQuest.hpp
)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(INSTALL_CMAKE_DIR cmake)
else()
  set(INSTALL_CMAKE_DIR lib/cmake/Athena)
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
install(DIRECTORY include/ DESTINATION ${INSTALL_INCLUDE_DIR}/Athena COMPONENT Athena)
install(TARGETS AthenaCore AthenaSakura AthenaWiiSave AthenaZelda
        DESTINATION ${INSTALL_LIB_DIR} EXPORT AthenaTargets COMPONENT Athena)
if(WIN32 AND NOT CYGWIN)
install(FILES Athena.ico DESTINATION ${INSTALL_LIB_DIR} COMPONENT Athena)
elseif(UNIX)
configure_file(libAthena.pc.in libAthena.pc @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/libAthena.pc 
        DESTINATION ${INSTALL_LIB_DIR}/pkgconfig COMPONENT Athena)
endif()

##################
# Package Export #
##################

# Add all targets to the build-tree export set
export(TARGETS AthenaCore AthenaSakura AthenaWiiSave AthenaZelda
  FILE "${PROJECT_BINARY_DIR}/AthenaTargets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE Athena)

# Create the AthenaConfig.cmake
# ... for the build tree
set(CONF_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/Athena")
set(CONF_ICON_DIR "${PROJECT_SOURCE_DIR}/Athena.ico")
configure_file(AthenaConfig.cmake.in "${PROJECT_BINARY_DIR}/AthenaConfig.cmake" @ONLY)
# ... for the install tree
file(RELATIVE_PATH REL_INCLUDE_DIR "${ABS_INSTALL_CMAKE_DIR}" "${ABS_INSTALL_INCLUDE_DIR}")
file(RELATIVE_PATH REL_ICON_DIR "${ABS_INSTALL_CMAKE_DIR}" "${ABS_INSTALL_LIB_DIR}/Athena.ico")
set(CONF_INCLUDE_DIR "\${ATHENA_CMAKE_DIR}/${REL_INCLUDE_DIR}/Athena")
set(CONF_ICON_DIR "\${ATHENA_CMAKE_DIR}/${REL_ICON_DIR}")
configure_file(AthenaConfig.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/AthenaConfig.cmake" @ONLY)
# ... for both
configure_file(AthenaConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/AthenaConfigVersion.cmake" @ONLY)

# Install AthenaConfig.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/AthenaConfig.cmake"
  "${PROJECT_BINARY_DIR}/AthenaConfigVersion.cmake"
  DESTINATION ${INSTALL_CMAKE_DIR} COMPONENT Athena)

# Install the export set for use with the install-tree
install(EXPORT AthenaTargets DESTINATION ${INSTALL_CMAKE_DIR} COMPONENT Athena)

################
# atdna import #
################

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/atdna/CMakeLists.txt")
  add_subdirectory(atdna)
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
set(CPACK_PACKAGE_INSTALL_DIRECTORY "Athena")
if(WIN32 AND NOT UNIX)
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Athena")
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

