!contains(ATHENA_CORE, true): {
    message("including core")
    include(AthenaCore.pri)
}

SOURCES += \
    $$PWD/src/Athena/Sprite.cpp \
    $$PWD/src/Athena/SpriteFile.cpp \
    $$PWD/src/Athena/SpriteFileReader.cpp \
    $$PWD/src/Athena/SpriteFileWriter.cpp \
    $$PWD/src/Athena/SpriteFrame.cpp \
    $$PWD/src/Athena/SpritePart.cpp

HEADERS += \
    $$PWD/include/Athena/Sprite.hpp \
    $$PWD/include/Athena/SpriteFile.hpp \
    $$PWD/include/Athena/SpriteFileReader.hpp \
    $$PWD/include/Athena/SpriteFileWriter.hpp \
    $$PWD/include/Athena/SpriteFrame.hpp \
    $$PWD/include/Athena/SpritePart.hpp
