CONFIG += staticlib c++11
QMAKE_CXXFLAGS += -std=c++11
TEMPLATE= lib
DESTDIR = ./lib
INCLUDEPATH += $$PWD/include

# Uncomment this if you wish to use Qt with libAthena
#DEFINES += ATHENA_USE_QT

contains(DEFINES, ATHENA_USE_QT){
    QT += qt core
} else {
    QT =
}

CONFIG(debug, debug|release): {
    DEFINES += DEBUG
    TARGET=Athena-d
    # We don't want the objects,
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/debug
}

CONFIG(release, release|debug): {
    DEFINES -= DEBUG
    TARGET=Athena
    # We don't want the objects,
    # in the project directory, so tell qmake
    # where to put them
    OBJECTS_DIR = obj/release
}

ATHENA_PRO=true

include(Athena.pri)


OTHER_FILES += \
    .travis.yml

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    libFiles.path = $$PREFIX/lib
    libFiles.files = $$PWD/lib/*
    headerFiles.files = $$PWD/include/*
    headerFiles.path = $$PREFIX/include/Athena
    INSTALLS += libFiles headerFiles
}

win32 {
   isEmpty(PREFIX) {
        PREFIX = $$PWD/pkg
   }

   libFiles.path = $$PREFIX/lib
   libFiles.files = $$PWD/lib/*
   headerFiles.path = $$PREFIX/include/Athena
   headerFiles.files = $$PWD/include/*
   INSTALLS += libFiles headerFiles
}


