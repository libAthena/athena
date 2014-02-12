INCLUDEPATH += $$PWD/include

QMAKE_CXXFLAGS = -std=c++0x

unix:LIBS += -lz
win32:LIBS += -lzlib
HEADERS += \
    $$PWD/include/utility.hpp \
    $$PWD/include/utf8.h \
    $$PWD/include/utf8/unchecked.h \
    $$PWD/include/utf8/core.h \
    $$PWD/include/utf8/checked.h \
    $$PWD/include/Types.hpp \
    $$PWD/include/TextStream.hpp \
    $$PWD/include/Stream.hpp \
    $$PWD/include/Mainpage.hpp \
    $$PWD/include/InvalidOperationException.hpp \
    $$PWD/include/IOException.hpp \
    $$PWD/include/FileNotFoundException.hpp \
    $$PWD/include/Exception.hpp \
    $$PWD/include/BinaryWriter.hpp \
    $$PWD/include/BinaryReader.hpp \
    $$PWD/include/WiiBanner.hpp \
    $$PWD/include/WiiFile.hpp \
    $$PWD/include/WiiSave.hpp \
    $$PWD/include/WiiSaveReader.hpp \
    $$PWD/include/WiiSaveWriter.hpp \
    $$PWD/include/aes.h \
    $$PWD/include/bn.h \
    $$PWD/include/ec.h \
    $$PWD/include/md5.h \
    $$PWD/include/sha1.h \
    $$PWD/include/ALTTPStructs.hpp \
    $$PWD/include/ALTTPQuest.hpp \
    $$PWD/include/ALTTPFileWriter.hpp \
    $$PWD/include/ALTTPFileReader.hpp \
    $$PWD/include/ALTTPFile.hpp \
    $$PWD/include/ALTTPEnums.hpp \
    $$PWD/include/MCFileReader.hpp \
    $$PWD/include/MCFile.hpp \
    $$PWD/include/MCFileWriter.hpp \
    $$PWD/include/ZQuestFileWriter.hpp \
    $$PWD/include/ZQuestFileReader.hpp \
    $$PWD/include/Compression.hpp \
	$$PWD/include/lzo.h \    
	$$PWD/include/WiiImage.hpp \
    $$PWD/include/ZQuestFile.hpp \
    $$PWD/include/Checksums.hpp \
    $$PWD/include/SkywardSwordFile.hpp \
    $$PWD/include/SkywardSwordFileReader.hpp \
    $$PWD/include/SkywardSwordFileWriter.hpp \
    $$PWD/include/SkywardSwordQuest.hpp \
    $$PWD/include/Sprite.hpp \
    $$PWD/include/SpriteFile.hpp \
    $$PWD/include/SpriteFileReader.hpp \
    $$PWD/include/SpriteFileWriter.hpp \
    $$PWD/include/SpriteFrame.hpp \
    $$PWD/include/SpritePart.hpp \
    ../libzelda/include/InvalidDataException.hpp

SOURCES += \
    $$PWD/src/utility.cpp \
    $$PWD/src/TextStream.cpp \
    $$PWD/src/Stream.cpp \
    $$PWD/src/BinaryWriter.cpp \
    $$PWD/src/BinaryReader.cpp \
    $$PWD/src/WiiBanner.cpp \
    $$PWD/src/WiiFile.cpp \
    $$PWD/src/WiiSave.cpp \
    $$PWD/src/WiiSaveReader.cpp \
    $$PWD/src/WiiSaveWriter.cpp \
    $$PWD/src/aes.c \
    $$PWD/src/bn.cpp \
    $$PWD/src/ec.cpp \
    $$PWD/src/md5.cpp \
    $$PWD/src/sha1.cpp \
    $$PWD/src/ALTTPQuest.cpp \
    $$PWD/src/ALTTPFileWriter.cpp \
    $$PWD/src/ALTTPFileReader.cpp \
    $$PWD/src/ALTTPFile.cpp \
    $$PWD/src/MCFileReader.cpp \
    $$PWD/src/MCFile.cpp \
    $$PWD/src/MCFileWriter.cpp \
    $$PWD/src/ZQuestFileWriter.cpp \
    $$PWD/src/ZQuestFileReader.cpp \
    $$PWD/src/Compression.cpp \
	$$PWD/src/lzo.c \
    $$PWD/src/WiiImage.cpp \
    $$PWD/src/ZQuestFile.cpp \
    $$PWD/src/Checksums.cpp \
    $$PWD/src/SkywardSwordFile.cpp \
    $$PWD/src/SkywardSwordFileReader.cpp \
    $$PWD/src/SkywardSwordFileWriter.cpp \
    $$PWD/src/SkywardSwordQuest.cpp \
    $$PWD/src/Sprite.cpp \
    $$PWD/src/SpriteFile.cpp \
    $$PWD/src/SpriteFileReader.cpp \
    $$PWD/src/SpriteFileWriter.cpp \
    $$PWD/src/SpriteFrame.cpp \
    $$PWD/src/SpritePart.cpp
