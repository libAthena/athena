!contains(ATHENA_CORE, true): {
    message("including core")
    include(AthenaCore.pri)
}

SOURCES += \
    $$PWD/src/Athena/WiiBanner.cpp \
    $$PWD/src/Athena/WiiFile.cpp \
    $$PWD/src/Athena/WiiImage.cpp \
    $$PWD/src/Athena/WiiSave.cpp \
    $$PWD/src/Athena/WiiSaveReader.cpp \
    $$PWD/src/Athena/WiiSaveWriter.cpp \
    $$PWD/src/bn.cpp \
    $$PWD/src/ec.cpp \
    $$PWD/src/md5.cpp \
    $$PWD/src/sha1.cpp \
    $$PWD/src/aes.c

HEADERS += \
    $$PWD/include/Athena/WiiBanner.hpp \
    $$PWD/include/Athena/WiiFile.hpp \
    $$PWD/include/Athena/WiiImage.hpp \
    $$PWD/include/Athena/WiiSave.hpp \
    $$PWD/include/Athena/WiiSaveReader.hpp \
    $$PWD/include/Athena/WiiSaveWriter.hpp \
    $$PWD/include/aes.h \
    $$PWD/include/bn.h \
    $$PWD/include/ec.h \
    $$PWD/include/md5.h \
    $$PWD/include/sha1.h
