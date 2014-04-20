INCLUDEPATH += $$PWD/include

unix:LIBS += -lz
win32:LIBS += -lzlib
QMAKE_CXXFLAGS += -std=c++11
CONFIG -= qt

SOURCES += \
    $$PWD/src/Athena/Utility.cpp \
    $$PWD/src/Athena/FileReader.cpp \
    $$PWD/src/Athena/FileWriter.cpp \
    $$PWD/src/Athena/BinaryReader.cpp \
    $$PWD/src/Athena/BinaryWriter.cpp \
    $$PWD/src/Athena/Global.cpp \
    $$PWD/src/Athena/ALTTPFile.cpp \
    $$PWD/src/Athena/ALTTPFileReader.cpp \
    $$PWD/Athena/ALTTPFileWriter.cpp \
    $$PWD/Athena/ALTTPQuest.cpp \
    $$PWD/Athena/Checksums.cpp \
    $$PWD/Athena/Compression.cpp \
    $$PWD/Athena/MCFile.cpp \
    $$PWD/Athena/MCFileReader.cpp \
    $$PWD/Athena/MCFileWriter.cpp \
    $$PWD/Athena/SkywardSwordFile.cpp \
    $$PWD/Athena/SkywardSwordFileReader.cpp \
    $$PWD/Athena/SkywardSwordFileWriter.cpp \
    $$PWD/Athena/SkywardSwordQuest.cpp \
    $$PWD/Athena/Sprite.cpp \
    $$PWD/Athena/SpriteFile.cpp \
    $$PWD/Athena/SpriteFileReader.cpp \
    $$PWD/Athena/SpriteFileWriter.cpp \
    $$PWD/Athena/SpriteFrame.cpp \
    $$PWD/Athena/SpritePart.cpp \
    $$PWD/Athena/WiiBanner.cpp \
    $$PWD/Athena/WiiFile.cpp \
    $$PWD/Athena/WiiImage.cpp \
    $$PWD/Athena/WiiSave.cpp \
    $$PWD/Athena/WiiSaveReader.cpp \
    $$PWD/Athena/WiiSaveWriter.cpp \
    $$PWD/Athena/ZQuestFile.cpp \
    $$PWD/Athena/ZQuestFileReader.cpp \
    $$PWD/Athena/ZQuestFileWriter.cpp \
    $$PWD/bn.cpp \
    $$PWD/ec.cpp \
    $$PWD/md5.cpp \
    $$PWD/sha1.cpp \
    $$PWD/aes.c \
    $$PWD/lzo.c

HEADERS += \
    $$PWD/Athena/Stream.hpp \
    $$PWD/Athena/Types.hpp \
    $$PWD/Athena/Utility.hpp \
    $$PWD/Athena/Global.hpp \
    $$PWD/Athena/Exception.hpp \
    $$PWD/Athena/FileNotFoundException.hpp \
    $$PWD/Athena/IOException.hpp \
    $$PWD/Athena/InvalidDataException.hpp \
    $$PWD/Athena/InvalidOperationException.hpp \
    $$PWD/Athena/FileReader.hpp \
    $$PWD/Athena/FileWriter.hpp \
    $$PWD/utf8.h \
    $$PWD/utf8/checked.h \
    $$PWD/utf8/core.h \
    $$PWD/utf8/unchecked.h \
    $$PWD/Athena/BinaryReader.hpp \
    $$PWD/Athena/BinaryWriter.hpp \
    $$PWD/Athena/NotImplementedException.hpp \
    $$PWD/aes.h \
    $$PWD/bn.h \
    $$PWD/ec.h \
    $$PWD/lzo.h \
    $$PWD/md5.h \
    $$PWD/sha1.h \
    $$PWD/Athena/ALTTPEnums.hpp \
    $$PWD/Athena/ALTTPFile.hpp \
    $$PWD/Athena/ALTTPFileReader.hpp \
    $$PWD/Athena/ALTTPFileWriter.hpp \
    $$PWD/Athena/ALTTPQuest.hpp \
    $$PWD/Athena/ALTTPStructs.hpp \
    $$PWD/Athena/Checksums.hpp \
    $$PWD/Athena/Compression.hpp \
    $$PWD/Athena/MCFile.hpp \
    $$PWD/Athena/MCFileReader.hpp \
    $$PWD/Athena/MCFileWriter.hpp \
    $$PWD/Athena/SkywardSwordFile.hpp \
    $$PWD/Athena/SkywardSwordFileReader.hpp \
    $$PWD/Athena/SkywardSwordFileWriter.hpp \
    $$PWD/Athena/SkywardSwordQuest.hpp \
    $$PWD/Athena/Sprite.hpp \
    $$PWD/Athena/SpriteFile.hpp \
    $$PWD/Athena/SpriteFileReader.hpp \
    $$PWD/Athena/SpriteFileWriter.hpp \
    $$PWD/Athena/SpriteFrame.hpp \
    $$PWD/Athena/SpritePart.hpp \
    $$PWD/Athena/WiiBanner.hpp \
    $$PWD/Athena/WiiFile.hpp \
    $$PWD/Athena/WiiImage.hpp \
    $$PWD/Athena/WiiSave.hpp \
    $$PWD/Athena/WiiSaveReader.hpp \
    $$PWD/Athena/WiiSaveWriter.hpp \
    $$PWD/Athena/ZQuestFile.hpp \
    $$PWD/Athena/ZQuestFileReader.hpp \
    $$PWD/Athena/ZQuestFileWriter.hpp

OTHER_FILES += \
    .travis.yml

