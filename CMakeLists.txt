cmake_minimum_required(VERSION 3.0)
project(Athena)

add_subdirectory(extern)
file(GLOB ATHENA_HEADERS include/Athena/*.hpp)

include_directories(include)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(WIN32)
list(APPEND CORE_EXTRA src/win32_largefilewrapper.c)
elseif(APPLE)
list(APPEND CORE_EXTRA src/osx_largefilewrapper.c)
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
    src/gekko_support.c
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
    include/utf8.h
    include/utf8/checked.h
    include/utf8/core.h
    include/utf8/unchecked.h
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
    src/aes.c

    include/Athena/WiiBanner.hpp
    include/Athena/WiiFile.hpp
    include/Athena/WiiImage.hpp
    include/Athena/WiiSave.hpp
    include/Athena/WiiSaveReader.hpp
    include/Athena/WiiSaveWriter.hpp
    include/aes.h
    include/bn.h
    include/ec.h
    include/md5.h
    include/sha1.h
)

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


install(DIRECTORY include DESTINATION include/Athena)
install(TARGETS AthenaCore AthenaSakura AthenaWiiSave AthenaZelda
        DESTINATION lib/Athena)

