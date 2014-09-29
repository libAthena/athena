CONFIG += staticlib
TEMPLATE= lib
DESTDIR = ./lib

# Uncomment this if you wish to use Qt with libAthena
#DEFINES += ATHENA_USE_QT

contains(DEFINES, ATHENA_USE_QT){
    QT += qt core
} else {
    QT =
}

CONFIG(debug, debug|release){
    DEFINES += DEBUG
    TARGET=Athena-d
    # We don't want the objects,
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/debug
}

CONFIG(release, release|debug){
    DEFINES -= DEBUG
    TARGET=Athena
    # We don't want the objects,
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/release
}

QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += include

SOURCES += \
    src/Athena/Utility.cpp \
    src/Athena/FileReader.cpp \
    src/Athena/FileWriter.cpp \
    src/Athena/BinaryReader.cpp \
    src/Athena/BinaryWriter.cpp \
    src/Athena/Global.cpp \
    src/Athena/ALTTPFile.cpp \
    src/Athena/ALTTPFileReader.cpp \
    src/Athena/ALTTPFileWriter.cpp \
    src/Athena/ALTTPQuest.cpp \
    src/Athena/Checksums.cpp \
    src/Athena/Compression.cpp \
    src/Athena/MCFile.cpp \
    src/Athena/MCFileReader.cpp \
    src/Athena/MCFileWriter.cpp \
    src/Athena/SkywardSwordFile.cpp \
    src/Athena/SkywardSwordFileReader.cpp \
    src/Athena/SkywardSwordFileWriter.cpp \
    src/Athena/SkywardSwordQuest.cpp \
    src/Athena/Sprite.cpp \
    src/Athena/SpriteFile.cpp \
    src/Athena/SpriteFileReader.cpp \
    src/Athena/SpriteFileWriter.cpp \
    src/Athena/SpriteFrame.cpp \
    src/Athena/SpritePart.cpp \
    src/Athena/WiiBanner.cpp \
    src/Athena/WiiFile.cpp \
    src/Athena/WiiImage.cpp \
    src/Athena/WiiSave.cpp \
    src/Athena/WiiSaveReader.cpp \
    src/Athena/WiiSaveWriter.cpp \
    src/Athena/ZQuestFile.cpp \
    src/Athena/ZQuestFileReader.cpp \
    src/Athena/ZQuestFileWriter.cpp \
    src/Athena/PHYSFSFileReader.cpp \
    src/LZ77/LZBase.cpp \
    src/LZ77/LZLookupTable.cpp \
    src/LZ77/LZType10.cpp \
    src/LZ77/LZType11.cpp \
    src/bn.cpp \
    src/ec.cpp \
    src/md5.cpp \
    src/sha1.cpp \
    src/aes.c \
    src/lzo.c

INCLUDEPATH += \
    include

HEADERS += \
    include/Athena/Global.hpp \
    include/Athena/Stream.hpp \
    include/Athena/Types.hpp \
    include/Athena/Utility.hpp \
    include/Athena/Exception.hpp \
    include/Athena/FileNotFoundException.hpp \
    include/Athena/IOException.hpp \
    include/Athena/InvalidDataException.hpp \
    include/Athena/InvalidOperationException.hpp \
    include/Athena/FileReader.hpp \
    include/Athena/FileWriter.hpp \
    include/Athena/BinaryReader.hpp \
    include/Athena/BinaryWriter.hpp \
    include/Athena/NotImplementedException.hpp \
    include/Athena/ALTTPEnums.hpp \
    include/Athena/ALTTPFile.hpp \
    include/Athena/ALTTPFileReader.hpp \
    include/Athena/ALTTPFileWriter.hpp \
    include/Athena/ALTTPQuest.hpp \
    include/Athena/ALTTPStructs.hpp \
    include/Athena/Checksums.hpp \
    include/Athena/Compression.hpp \
    include/Athena/MCFile.hpp \
    include/Athena/MCFileReader.hpp \
    include/Athena/MCFileWriter.hpp \
    include/Athena/SkywardSwordFile.hpp \
    include/Athena/SkywardSwordFileReader.hpp \
    include/Athena/SkywardSwordFileWriter.hpp \
    include/Athena/SkywardSwordQuest.hpp \
    include/Athena/Sprite.hpp \
    include/Athena/SpriteFile.hpp \
    include/Athena/SpriteFileReader.hpp \
    include/Athena/SpriteFileWriter.hpp \
    include/Athena/SpriteFrame.hpp \
    include/Athena/SpritePart.hpp \
    include/Athena/WiiBanner.hpp \
    include/Athena/WiiFile.hpp \
    include/Athena/WiiImage.hpp \
    include/Athena/WiiSave.hpp \
    include/Athena/WiiSaveReader.hpp \
    include/Athena/WiiSaveWriter.hpp \
    include/Athena/ZQuestFile.hpp \
    include/Athena/ZQuestFileReader.hpp \
    include/Athena/ZQuestFileWriter.hpp \
    include/Athena/PHYSFSFileReader.hpp \
    include/LZ77/LZBase.hpp \
    include/LZ77/LZLookupTable.hpp \
    include/LZ77/LZType10.hpp \
    include/LZ77/LZType11.hpp \
    include/utf8.h \
    include/utf8/checked.h \
    include/utf8/core.h \
    include/utf8/unchecked.h \
    include/aes.h \
    include/bn.h \
    include/ec.h \
    include/lzo.h \
    include/md5.h \
    include/sha1.h

OTHER_FILES += \
    .travis.yml

isEmpty(PREFIX) {
    PREFIX = /usr/local
}

unix {
    target.path = $$PREFIX/lib
    target.files = $$PWD/lib/*
    headerFiles.files = $$PWD/include/*
    headerFiles.path = $$PREFIX/include/Athena
    pkgconf.files = libAthena.pc
    pkgconf.path = $$PREFIX/lib/pkgconfig
    INSTALLS += target headerFiles pkgconf
}
