!contains($$INCLUDEPATH, $$PWD/include): {
    INCLUDEPATH += $$PWD/include
    unix:LIBS += -lz
    win32:LIBS += -lzlib
    QMAKE_CXXFLAGS += -std=c++11
}

SOURCES += \
    $$PWD/src/Athena/Utility.cpp \
    $$PWD/src/Athena/FileReader.cpp \
    $$PWD/src/Athena/FileWriter.cpp \
    $$PWD/src/Athena/BinaryReader.cpp \
    $$PWD/src/Athena/BinaryWriter.cpp \
    $$PWD/src/Athena/Global.cpp \
    $$PWD/src/Athena/Checksums.cpp \
    $$PWD/src/Athena/Compression.cpp \
    $$PWD/src/LZ77/LZLookupTable.cpp \
    $$PWD/src/LZ77/LZType10.cpp \
    $$PWD/src/LZ77/LZType11.cpp \
    $$PWD/src/LZ77/LZBase.cpp

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
    $$PWD/include/Athena/BinaryReader.hpp \
    $$PWD/include/Athena/BinaryWriter.hpp \
    $$PWD/include/Athena/NotImplementedException.hpp \
    $$PWD/include/Athena/Checksums.hpp \
    $$PWD/include/Athena/Compression.hpp \
    $$PWD/include/LZ77/LZBase.hpp \
    $$PWD/include/LZ77/LZLookupTable.hpp \
    $$PWD/include/LZ77/LZType10.hpp \
    $$PWD/include/LZ77/LZType11.hpp \
    $$PWD/include/utf8.h \
    $$PWD/include/utf8/checked.h \
    $$PWD/include/utf8/core.h \
    $$PWD/include/utf8/unchecked.h

win32:HEADERS += \
    $$PWD/include/win32_largefilewrapper.h
