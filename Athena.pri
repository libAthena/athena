INCLUDEPATH += $$PWD/include

unix:LIBS += -lz
win32:LIBS += -lzlib
QMAKE_CXXFLAGS += -std=c++11

SOURCES += \
    $$PWD/src/Athena/Utility.cpp \
    $$PWD/src/Athena/FileReader.cpp \
    $$PWD/src/Athena/FileWriter.cpp \
    $$PWD/src/Athena/BinaryReader.cpp \
    $$PWD/src/Athena/BinaryWriter.cpp \
    $$PWD/src/Athena/Global.cpp \
    $$PWD/src/Athena/ALTTPFile.cpp \
    $$PWD/src/Athena/ALTTPFileReader.cpp \
    $$PWD/src/Athena/ALTTPFileWriter.cpp \
    $$PWD/src/Athena/ALTTPQuest.cpp \
    $$PWD/src/Athena/Checksums.cpp \
    $$PWD/src/Athena/Compression.cpp \
    $$PWD/src/Athena/MCFile.cpp \
    $$PWD/src/Athena/MCFileReader.cpp \
    $$PWD/src/Athena/MCFileWriter.cpp \
    $$PWD/src/Athena/SkywardSwordFile.cpp \
    $$PWD/src/Athena/SkywardSwordFileReader.cpp \
    $$PWD/src/Athena/SkywardSwordFileWriter.cpp \
    $$PWD/src/Athena/SkywardSwordQuest.cpp \
    $$PWD/src/Athena/Sprite.cpp \
    $$PWD/src/Athena/SpriteFile.cpp \
    $$PWD/src/Athena/SpriteFileReader.cpp \
    $$PWD/src/Athena/SpriteFileWriter.cpp \
    $$PWD/src/Athena/SpriteFrame.cpp \
    $$PWD/src/Athena/SpritePart.cpp \
    $$PWD/src/Athena/WiiBanner.cpp \
    $$PWD/src/Athena/WiiFile.cpp \
    $$PWD/src/Athena/WiiImage.cpp \
    $$PWD/src/Athena/WiiSave.cpp \
    $$PWD/src/Athena/WiiSaveReader.cpp \
    $$PWD/src/Athena/WiiSaveWriter.cpp \
    $$PWD/src/Athena/ZQuestFile.cpp \
    $$PWD/src/Athena/ZQuestFileReader.cpp \
    $$PWD/src/Athena/ZQuestFileWriter.cpp \
    $$PWD/src/bn.cpp \
    $$PWD/src/ec.cpp \
    $$PWD/src/md5.cpp \
    $$PWD/src/sha1.cpp \
    $$PWD/src/aes.c \
    $$PWD/src/lzo.c
win32:SOURCES += $$PWD/src/win32_largefilewrapper.c

HEADERS += \
    $$PWD/include/Athena/Stream.hpp \
    $$PWD/include/Athena/Types.hpp \
    $$PWD/include/Athena/Utility.hpp \
    $$PWD/include/Athena/Global.hpp \
    $$PWD/include/Athena/Exception.hpp \
    $$PWD/include/Athena/FileNotFoundException.hpp \
    $$PWD/include/Athena/IOException.hpp \
    $$PWD/include/Athena/InvalidDataException.hpp \
    $$PWD/include/Athena/InvalidOperationException.hpp \
    $$PWD/include/Athena/FileReader.hpp \
    $$PWD/include/Athena/FileWriter.hpp \
    $$PWD/include/utf8.h \
    $$PWD/include/utf8/checked.h \
    $$PWD/include/utf8/core.h \
    $$PWD/include/utf8/unchecked.h \
    $$PWD/include/Athena/BinaryReader.hpp \
    $$PWD/include/Athena/BinaryWriter.hpp \
    $$PWD/include/Athena/NotImplementedException.hpp \
    $$PWD/include/aes.h \
    $$PWD/include/bn.h \
    $$PWD/include/ec.h \
    $$PWD/include/lzo.h \
    $$PWD/include/md5.h \
    $$PWD/include/sha1.h \
    $$PWD/include/Athena/ALTTPEnums.hpp \
    $$PWD/include/Athena/ALTTPFile.hpp \
    $$PWD/include/Athena/ALTTPFileReader.hpp \
    $$PWD/include/Athena/ALTTPFileWriter.hpp \
    $$PWD/include/Athena/ALTTPQuest.hpp \
    $$PWD/include/Athena/ALTTPStructs.hpp \
    $$PWD/include/Athena/Checksums.hpp \
    $$PWD/include/Athena/Compression.hpp \
    $$PWD/include/Athena/MCFile.hpp \
    $$PWD/include/Athena/MCFileReader.hpp \
    $$PWD/include/Athena/MCFileWriter.hpp \
    $$PWD/include/Athena/SkywardSwordFile.hpp \
    $$PWD/include/Athena/SkywardSwordFileReader.hpp \
    $$PWD/include/Athena/SkywardSwordFileWriter.hpp \
    $$PWD/include/Athena/SkywardSwordQuest.hpp \
    $$PWD/include/Athena/Sprite.hpp \
    $$PWD/include/Athena/SpriteFile.hpp \
    $$PWD/include/Athena/SpriteFileReader.hpp \
    $$PWD/include/Athena/SpriteFileWriter.hpp \
    $$PWD/include/Athena/SpriteFrame.hpp \
    $$PWD/include/Athena/SpritePart.hpp \
    $$PWD/include/Athena/WiiBanner.hpp \
    $$PWD/include/Athena/WiiFile.hpp \
    $$PWD/include/Athena/WiiImage.hpp \
    $$PWD/include/Athena/WiiSave.hpp \
    $$PWD/include/Athena/WiiSaveReader.hpp \
    $$PWD/include/Athena/WiiSaveWriter.hpp \
    $$PWD/include/Athena/ZQuestFile.hpp \
    $$PWD/include/Athena/ZQuestFileReader.hpp \
    $$PWD/include/Athena/ZQuestFileWriter.hpp

win32:HEADERS += \
    $$PWD/Athena/include/win32_largefilewrapper.h

OTHER_FILES += \
    .travis.yml

