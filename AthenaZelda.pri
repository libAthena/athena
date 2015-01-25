!contains($$INCLUDEPATH, $$PWD/include): {
	include(AthenaCore.pri)
}

SOURCES += \
    $$PWD/src/Athena/ALTTPFile.cpp \
    $$PWD/src/Athena/ALTTPFileReader.cpp \
    $$PWD/src/Athena/ALTTPFileWriter.cpp \
    $$PWD/src/Athena/ALTTPQuest.cpp \
    $$PWD/src/Athena/MCFile.cpp \
    $$PWD/src/Athena/MCFileReader.cpp \
    $$PWD/src/Athena/MCFileWriter.cpp \
    $$PWD/src/Athena/MCSlot.cpp \
    $$PWD/src/Athena/SkywardSwordFile.cpp \
    $$PWD/src/Athena/SkywardSwordFileReader.cpp \
    $$PWD/src/Athena/SkywardSwordFileWriter.cpp \
    $$PWD/src/Athena/SkywardSwordQuest.cpp \
    $$PWD/src/Athena/ZQuestFile.cpp \
    $$PWD/src/Athena/ZQuestFileReader.cpp \
    $$PWD/src/Athena/ZQuestFileWriter.cpp

HEADERS += \
    $$PWD/include/Athena/ALTTPEnums.hpp \
    $$PWD/include/Athena/ALTTPFile.hpp \
    $$PWD/include/Athena/ALTTPFileReader.hpp \
    $$PWD/include/Athena/ALTTPFileWriter.hpp \
    $$PWD/include/Athena/ALTTPQuest.hpp \
    $$PWD/include/Athena/ALTTPStructs.hpp \
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
    $$PWD/include/Athena/ZQuestFile.hpp \
    $$PWD/include/Athena/ZQuestFileReader.hpp \
    $$PWD/include/Athena/ZQuestFileWriter.hpp
