TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QT =
DEFINES += __STDC_LIMIT_MACROS=1 __STDC_CONSTANT_MACROS=1
QMAKE_CXXFLAGS += -fno-rtti
QMAKE_CXXFLAGS_WARN_ON = -Wno-unused-parameter

INCLUDEPATH += ../include

CONFIG(debug, debug|release) {
    # FOR FULL DEBUGGING, PLEASE UNCOMMENT THESE AND POINT THEM TO
    # THE INSTALL PREFIX OF YOUR OWN DEBUG BUILD OF LLVM/CLANG!!
    #INCLUDEPATH += /home/jacko/llvm-build/usr/include
    #LIBS += -L/home/jacko/llvm-build/usr/lib
    LIBS += -g
} else {
    LIBS += -flto
}

SOURCES += \
    main.cpp

LIBS += -lclangFrontend -lclangTooling -lclangDriver \
        -lclangSerialization -lclangParse -lclangSema \
        -lclangAnalysis -lclangEdit \
        -lclangAST -lclangLex -lclangBasic \
        -lLLVMOption -lLLVMMCParser -lLLVMBitReader \
        -lLLVMMC -lLLVMSupport -lz -lpthread -lcurses -ldl

HEADERS += \
    test.hpp

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    isEmpty(INSTALL_PREFIX) {
        INSTALL_PREFIX = /usr/local
    }
    utilFiles.path = $$PREFIX/bin
    utilFiles.files = $$OUT_PWD/atdna
    INSTALLS += utilFiles
}

win32 {
    isEmpty(PREFIX) {
        PREFIX = $$PWD/pkg
    }
    isEmpty(INSTALL_PREFIX) {
        INSTALL_PREFIX = $$PWD/pkg
    }
    utilFiles.path = $$PREFIX/bin
    utilFiles.files = $$OUT_PWD/atdna
    INSTALLS += utilFiles
}

DEFINES += INSTALL_PREFIX="$$INSTALL_PREFIX"

