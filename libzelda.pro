CONFIG += staticlib
TEMPLATE= lib
DESTDIR = ./lib

CONFIG(debug, debug|release){
    DEFINES += DEBUG
    TARGET=zelda-d
    # We don't want the objects,
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/debug
}

CONFIG(release, release|debug){
    DEFINES -= DEBUG
    TARGET=zelda
    # We don't want the objects,
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/release
}

QMAKE_CXXFLAGS = -std=c++0x
INCLUDEPATH += include

HEADERS += \
    include/utility.hpp \
    include/utf8.h \
    include/utf8/unchecked.h \
    include/utf8/core.h \
    include/utf8/checked.h \
    include/Types.hpp \
    include/TextStream.hpp \
    include/Stream.hpp \
    include/Mainpage.hpp \
    include/InvalidOperationException.hpp \
    include/IOException.hpp \
    include/FileNotFoundException.hpp \
    include/Exception.hpp \
    include/BinaryWriter.hpp \
    include/BinaryReader.hpp \
    include/WiiBanner.hpp \
    include/WiiFile.hpp \
    include/WiiSave.hpp \
    include/WiiSaveReader.hpp \
    include/WiiSaveWriter.hpp \
    include/aes.h \
    include/bn.h \
    include/ec.h \
    include/md5.h \
    include/sha1.h \
    include/ALTTPStructs.hpp \
    include/ALTTPQuest.hpp \
    include/ALTTPFileWriter.hpp \
    include/ALTTPFileReader.hpp \
    include/ALTTPFile.hpp \
    include/ALTTPEnums.hpp \
    include/MCFileReader.hpp \
    include/MCFile.hpp \
    include/MCFileWriter.hpp \
    include/ZQuestFileWriter.hpp \
    include/ZQuestFileReader.hpp \
    include/Compression.hpp \
    include/WiiImage.hpp \
    include/ZQuestFile.hpp \
    include/Checksums.hpp \
    include/SkywardSwordFile.hpp \
    include/SkywardSwordFileReader.hpp \
    include/SkywardSwordFileWriter.hpp \
    include/SkywardSwordQuest.hpp \
    include/Sprite.hpp \
    include/SpriteFile.hpp \
    include/SpriteFileReader.hpp \
    include/SpriteFileWriter.hpp \
    include/SpriteFrame.hpp \
    include/SpritePart.hpp

SOURCES += \
    src/utility.cpp \
    src/TextStream.cpp \
    src/Stream.cpp \
    src/BinaryWriter.cpp \
    src/BinaryReader.cpp \
    src/WiiBanner.cpp \
    src/WiiFile.cpp \
    src/WiiSave.cpp \
    src/WiiSaveReader.cpp \
    src/WiiSaveWriter.cpp \
    src/aes.c \
    src/bn.cpp \
    src/ec.cpp \
    src/md5.cpp \
    src/sha1.cpp \
    src/ALTTPQuest.cpp \
    src/ALTTPFileWriter.cpp \
    src/ALTTPFileReader.cpp \
    src/ALTTPFile.cpp \
    src/MCFileReader.cpp \
    src/MCFile.cpp \
    src/MCFileWriter.cpp \
    src/ZQuestFileWriter.cpp \
    src/ZQuestFileReader.cpp \
    src/Compression.cpp \
    src/WiiImage.cpp \
    src/ZQuestFile.cpp \
    src/Checksums.cpp \
    src/SkywardSwordFile.cpp \
    src/SkywardSwordFileReader.cpp \
    src/SkywardSwordFileWriter.cpp \
    src/SkywardSwordQuest.cpp \
    src/Sprite.cpp \
    src/SpriteFile.cpp \
    src/SpriteFileReader.cpp \
    src/SpriteFileWriter.cpp \
    src/SpriteFrame.cpp \
    src/SpritePart.cpp
