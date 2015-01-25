!contains($$INCLUDEPATH, $$PWD/include): {
	include(AthenaCore.pri)
}

SOURCES += \
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
    $$PWD/src/aes.c

HEADERS += \
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
    $$PWD/include/aes.h \
    $$PWD/include/bn.h \
    $$PWD/include/ec.h \
    $$PWD/include/lzo.h \
    $$PWD/include/md5.h \
    $$PWD/include/sha1.h
