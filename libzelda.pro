CONFIG += staticlib
TEMPLATE=lib
TARGET=zelda
QMAKE_CXXFLAGS = -std=c++11
INCLUDEPATH += include

HEADERS += \
    include/utility.hpp \
    include/utf8.h \
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
    include/utf8/unchecked.h \
    include/utf8/core.h \
    include/utf8/checked.h

SOURCES += \
    src/utility.cpp \
    src/TextStream.cpp \
    src/Stream.cpp \
    src/BinaryWriter.cpp \
    src/BinaryReader.cpp

