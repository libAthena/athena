CONFIG += staticlib
TEMPLATE=lib
DESTDIR = ./

CONFIG(debug, debug|release){
    DEFINES += DEBUG
    TARGET=zelda-d
}

CONFIG(release, release|debug){
    DEFINES -= DEBUG
    TARGET=zelda
}

QMAKE_CXXFLAGS += -std=c++0x
INCLUDEPATH += include

HEADERS += \
    include/utility.hpp \
    include/utf8.h \
    include/utf8/unchecked.h \
    include/utf8/core.h \
    include/utf8/checked.h \
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
    include/WiiBanner.hpp \
    include/WiiFile.hpp \
    include/WiiSave.hpp \
    include/WiiSaveReader.hpp \
    include/WiiSaveWriter.hpp \
    include/aes.h \
    include/bn.h \
    include/ec.h \
    include/md5.h \
    include/sha1.h \
    include/ALTTPStructs.hpp \
    include/ALTTPQuest.hpp \
    include/ALTTPFileWriter.hpp \
    include/ALTTPFileReader.hpp \
    include/ALTTPFile.hpp \
    include/ALTTPEnums.hpp \
    include/MCFileReader.hpp \
    include/MCFile.hpp \
    include/MCFileWriter.hpp \
    include/ZQuestFileWriter.hpp \
    include/ZQuestFileReader.hpp \
    include/Compression.hpp \
    include/WiiImage.hpp \
    include/ZQuestFile.hpp \
    include/SSQuest.hpp \
    include/SSFileWriter.hpp \
    include/SSFileReader.hpp \
    include/SSFile.hpp

SOURCES += \
    src/utility.cpp \
    src/TextStream.cpp \
    src/Stream.cpp \
    src/BinaryWriter.cpp \
    src/BinaryReader.cpp \
    src/WiiBanner.cpp \
    src/WiiFile.cpp \
    src/WiiSave.cpp \
    src/WiiSaveReader.cpp \
    src/WiiSaveWriter.cpp \
    src/aes.c \
    src/bn.cpp \
    src/ec.cpp \
    src/md5.c \
    src/sha1.cpp \
    src/ALTTPQuest.cpp \
    src/ALTTPFileWriter.cpp \
    src/ALTTPFileReader.cpp \
    src/ALTTPFile.cpp \
    src/MCFileReader.cpp \
    src/MCFile.cpp \
    src/MCFileWriter.cpp \
    src/ZQuestFileWriter.cpp \
    src/ZQuestFileReader.cpp \
    src/Compression.cpp \
    src/WiiImage.cpp \
    src/ZQuestFile.cpp \
    src/SSQuest.cpp \
    src/SSFileWriter.cpp \
    src/SSFileReader.cpp \
    src/SSFile.cpp

system("exec doxygen libzelda.conf")
#system("cd doc/latex && make")
system("cd ../../")
