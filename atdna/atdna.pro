TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QT =
DEFINES += __STDC_LIMIT_MACROS=1 __STDC_CONSTANT_MACROS=1
QMAKE_CXXFLAGS += -fno-rtti
QMAKE_CXXFLAGS_WARN_ON = -Wno-unused-parameter

INCLUDEPATH += ../include /run/media/jacko/Extra/llvm-build/usr/include

SOURCES += \
    main.cpp

LIBS += -L/run/media/jacko/Extra/llvm-build/usr/lib \
        -lclangFrontendTool -lclangFrontend -lclangTooling -lclangDriver \
        -lclangSerialization -lclangCodeGen -lclangParse -lclangSema \
        -lclangRewriteFrontend -lclangRewrite -lclangAnalysis -lclangEdit \
        -lclangAST -lclangLex -lclangBasic \
        -lLLVMLTO -lLLVMObjCARCOpts -lLLVMLinker -lLLVMBitWriter -lLLVMIRReader \
        -lLLVMXCoreDisassembler -lLLVMXCoreCodeGen -lLLVMXCoreDesc -lLLVMXCoreInfo \
        -lLLVMXCoreAsmPrinter -lLLVMSystemZDisassembler -lLLVMSystemZCodeGen \
        -lLLVMSystemZAsmParser -lLLVMSystemZDesc -lLLVMSystemZInfo -lLLVMSystemZAsmPrinter \
        -lLLVMSparcDisassembler -lLLVMSparcCodeGen -lLLVMSparcAsmParser -lLLVMSparcDesc \
        -lLLVMSparcInfo -lLLVMSparcAsmPrinter -lLLVMPowerPCDisassembler -lLLVMPowerPCCodeGen \
        -lLLVMPowerPCAsmParser -lLLVMPowerPCDesc -lLLVMPowerPCInfo -lLLVMPowerPCAsmPrinter \
        -lLLVMNVPTXCodeGen -lLLVMNVPTXDesc -lLLVMNVPTXInfo -lLLVMNVPTXAsmPrinter \
        -lLLVMMSP430CodeGen -lLLVMMSP430Desc -lLLVMMSP430Info -lLLVMMSP430AsmPrinter \
        -lLLVMMipsDisassembler -lLLVMMipsCodeGen -lLLVMMipsAsmParser -lLLVMMipsDesc \
        -lLLVMMipsInfo -lLLVMMipsAsmPrinter -lLLVMHexagonDisassembler -lLLVMHexagonCodeGen \
        -lLLVMHexagonDesc -lLLVMHexagonInfo -lLLVMCppBackendCodeGen -lLLVMCppBackendInfo \
        -lLLVMBPFCodeGen -lLLVMBPFDesc -lLLVMBPFInfo -lLLVMBPFAsmPrinter -lLLVMARMDisassembler \
        -lLLVMARMCodeGen -lLLVMARMAsmParser -lLLVMARMDesc -lLLVMARMInfo -lLLVMARMAsmPrinter \
        -lLLVMAMDGPUCodeGen -lLLVMAMDGPUAsmParser -lLLVMAMDGPUDesc -lLLVMAMDGPUInfo \
        -lLLVMAMDGPUAsmPrinter -lLLVMAArch64Disassembler -lLLVMAArch64CodeGen -lLLVMAArch64AsmParser \
        -lLLVMAArch64Desc -lLLVMAArch64Info -lLLVMAArch64AsmPrinter -lLLVMAArch64Utils \
        -lLLVMMIRParser -lLLVMAsmParser -lLLVMLibDriver -lLLVMOption -lLLVMDebugInfoPDB -lLLVMTableGen \
        -lLLVMOrcJIT -lLLVMLineEditor -lLLVMInstrumentation -lLLVMX86Disassembler -lLLVMX86AsmParser \
        -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMX86Desc -lLLVMMCDisassembler \
        -lLLVMX86Info -lLLVMX86AsmPrinter -lLLVMX86Utils -lLLVMMCJIT -lLLVMDebugInfoDWARF \
        -lLLVMPasses -lLLVMipo -lLLVMVectorize -lLLVMInterpreter -lLLVMExecutionEngine \
        -lLLVMRuntimeDyld -lLLVMCodeGen -lLLVMTarget -lLLVMScalarOpts -lLLVMProfileData \
        -lLLVMObject -lLLVMMCParser -lLLVMBitReader -lLLVMInstCombine -lLLVMTransformUtils \
        -lLLVMipa -lLLVMMC -lLLVMAnalysis -lLLVMCore -lLLVMSupport -lz -lpthread -lcurses -ldl -g

HEADERS += \
    test.hpp
