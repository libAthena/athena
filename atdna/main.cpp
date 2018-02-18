#include <cstdio>
#include <iostream>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/Utils.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Sema/Sema.h"
#include "clang/AST/RecordLayout.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/TypeLoc.h"
#include "clang/Basic/Version.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/CommandLine.h"

using namespace std::literals;

static unsigned AthenaError = 0;
#define ATHENA_DNA_BASETYPE "struct athena::io::DNA"

#ifndef INSTALL_PREFIX
#define INSTALL_PREFIX /usr/local
#endif
#define XSTR(s) STR(s)
#define STR(s) #s

static llvm::cl::opt<bool> Help("h", llvm::cl::desc("Alias for -help"), llvm::cl::Hidden);

static llvm::cl::opt<bool> Verbose("v", llvm::cl::desc("verbose mode"));

static llvm::cl::OptionCategory ATDNAFormatCategory("atdna options");

static llvm::cl::opt<std::string> OutputFilename("o",
                                                 llvm::cl::desc("Specify output filename"),
                                                 llvm::cl::value_desc("filename"),
                                                 llvm::cl::Prefix);

static llvm::cl::opt<bool> FExceptions("fexceptions",
                                       llvm::cl::desc("Enable C++ Exceptions"));
static llvm::cl::opt<bool> FMSCompat("fms-compatibility",
                                     llvm::cl::desc("Enable MS header compatibility"));
static llvm::cl::opt<std::string> FMSCompatVersion("fms-compatibility-version",
    llvm::cl::desc("Specify MS compatibility version (18.00 for VS2013, 19.00 for VS2015)"));

static llvm::cl::list<std::string> InputFilenames(llvm::cl::Positional,
                                                  llvm::cl::desc("<Input files>"),
                                                  llvm::cl::ZeroOrMore);

static llvm::cl::list<std::string> IncludeSearchPaths("I",
                                                      llvm::cl::desc("Header search path"),
                                                      llvm::cl::Prefix);

static llvm::cl::list<std::string> SystemIncludeSearchPaths("isystem",
                                                            llvm::cl::desc("System Header search path"));

static llvm::cl::opt<std::string> StandardCXXLib("stdlib",
                                                 llvm::cl::desc("Standard C++ library"));

static llvm::cl::opt<bool> DepFile("MD", llvm::cl::desc("Make Dependency file"));

static llvm::cl::opt<std::string> DepFileOut("MF",
                                             llvm::cl::desc("Dependency file out path"));

static llvm::cl::list<std::string> DepFileTargets("MT",
                                                  llvm::cl::desc("Dependency file targets"));

static llvm::cl::list<std::string> SystemIncRoot("isysroot",
                                                 llvm::cl::desc("System include root"));

static llvm::cl::list<std::string> PreprocessorDefines("D",
                                                       llvm::cl::desc("Preprocessor define"),
                                                       llvm::cl::Prefix);

static llvm::cl::opt<bool> EmitIncludes("emit-includes",
                                        llvm::cl::desc("Emit DNA for included files (not just main file)"));

/* LLVM 3.7 changed the stream type */
#if LLVM_VERSION_MAJOR > 3 || (LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR >= 7)
using StreamOut = llvm::raw_pwrite_stream;
#else
using StreamOut = llvm::raw_fd_ostream;
#endif

class ATDNAEmitVisitor : public clang::RecursiveASTVisitor<ATDNAEmitVisitor>
{
    clang::ASTContext& context;
    StreamOut& fileOut;

    bool isDNARecord(const clang::CXXRecordDecl* record, std::string& baseDNA)
    {
        for (const clang::CXXBaseSpecifier& base : record->bases())
        {
            const clang::QualType qtp = base.getType().getCanonicalType();
            if (!qtp.getAsString().compare(0, sizeof(ATHENA_DNA_BASETYPE)-1, ATHENA_DNA_BASETYPE))
                return true;
        }
        for (const clang::CXXBaseSpecifier& base : record->bases())
        {
            clang::QualType qtp = base.getType().getCanonicalType();
            const clang::Type* tp = qtp.getTypePtrOrNull();
            if (tp)
            {
                const clang::CXXRecordDecl* rDecl = tp->getAsCXXRecordDecl();
                if (rDecl)
                {
                    if (isDNARecord(rDecl, baseDNA))
                    {
                        bool hasRead = false;
                        bool hasWrite = false;
                        for (const clang::CXXMethodDecl* method : rDecl->methods())
                        {
                            std::string compName = method->getDeclName().getAsString();
                            if (!compName.compare("read"))
                                hasRead = true;
                            else if (!compName.compare("write"))
                                hasWrite = true;
                        }
                        if (hasRead && hasWrite)
                            baseDNA = rDecl->getQualifiedNameAsString();
                        return true;
                    }
                }
            }
        }
        return false;
    }

    int64_t GetSizeValue(const clang::Type* theType, unsigned width)
    {
        if (theType->isEnumeralType())
        {
            clang::EnumType* eType = (clang::EnumType*)theType;
            clang::EnumDecl* eDecl = eType->getDecl();
            theType = eDecl->getIntegerType().getCanonicalType().getTypePtr();

            const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
            if (bType->isBooleanType())
            {
                return 1;
            }
            else if (bType->isUnsignedInteger() || bType->isSignedInteger())
            {
                return width / 8;
            }
        }
        else if (theType->isBuiltinType())
        {
            const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
            if (bType->isBooleanType())
            {
                return 1;
            }
            else if (bType->isUnsignedInteger() || bType->isSignedInteger() || bType->isFloatingPoint())
            {
                return width / 8;
            }
        }
        else if (theType->isRecordType())
        {
            const clang::CXXRecordDecl* rDecl = theType->getAsCXXRecordDecl();
            for (const clang::FieldDecl* field : rDecl->fields())
            {
                if (!field->getName().compare("clangVec"))
                {
                    const clang::VectorType* vType = (clang::VectorType*)field->getType().getTypePtr();
                    if (vType->isVectorType())
                    {
                        const clang::BuiltinType* eType = (clang::BuiltinType*)vType->getElementType().getTypePtr();
                        const uint64_t width = context.getTypeInfo(eType).Width;
                        if (!eType->isBuiltinType() || !eType->isFloatingPoint() ||
                            (width != 32 && width != 64))
                            continue;
                        if (vType->getNumElements() == 2)
                        {
                            return width / 8 * 2;
                        }
                        else if (vType->getNumElements() == 3)
                        {
                            return width / 8 * 3;
                        }
                        else if (vType->getNumElements() == 4)
                        {
                            return width / 8 * 4;
                        }
                    }
                }
            }
        }
        return 0;
    }

#if 0
    std::string GetOpString(const clang::Type* theType, unsigned width,
                            const std::string& fieldName, bool writerPass,
                            const std::string& funcPrefix, bool& isDNATypeOut)
    {
        isDNATypeOut = false;
        if (writerPass)
        {
            if (theType->isEnumeralType())
            {
                clang::EnumType* eType = (clang::EnumType*)theType;
                clang::EnumDecl* eDecl = eType->getDecl();
                theType = eDecl->getIntegerType().getCanonicalType().getTypePtr();

                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isBooleanType())
                {
                    return ATHENA_DNA_WRITER ".writeBool(bool(" + fieldName + "));";
                }
                else if (bType->isUnsignedInteger())
                {
                    if (width == 8)
                        return ATHENA_DNA_WRITER ".writeUByte(atUint8(" + fieldName + "));";
                    else if (width == 16)
                        return ATHENA_DNA_WRITER ".writeUint16" + funcPrefix + "(atUint16(" + fieldName + "));";
                    else if (width == 32)
                        return ATHENA_DNA_WRITER ".writeUint32" + funcPrefix + "(atUint32(" + fieldName + "));";
                    else if (width == 64)
                        return ATHENA_DNA_WRITER ".writeUint64" + funcPrefix + "(atUint64(" + fieldName + "));";
                }
                else if (bType->isSignedInteger())
                {
                    if (width == 8)
                        return ATHENA_DNA_WRITER ".writeByte(atInt8(" + fieldName + "));";
                    else if (width == 16)
                        return ATHENA_DNA_WRITER ".writeInt16" + funcPrefix + "(atInt16(" + fieldName + "));";
                    else if (width == 32)
                        return ATHENA_DNA_WRITER ".writeInt32" + funcPrefix + "(atInt32(" + fieldName + "));";
                    else if (width == 64)
                        return ATHENA_DNA_WRITER ".writeInt64" + funcPrefix + "(atInt64(" + fieldName + "));";
                }
            }
            else if (theType->isBuiltinType())
            {
                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isBooleanType())
                {
                    return ATHENA_DNA_WRITER ".writeBool(" + fieldName + ");";
                }
                else if (bType->isUnsignedInteger())
                {
                    if (width == 8)
                        return ATHENA_DNA_WRITER ".writeUByte(" + fieldName + ");";
                    else if (width == 16)
                        return ATHENA_DNA_WRITER ".writeUint16" + funcPrefix + "(" + fieldName + ");";
                    else if (width == 32)
                        return ATHENA_DNA_WRITER ".writeUint32" + funcPrefix + "(" + fieldName + ");";
                    else if (width == 64)
                        return ATHENA_DNA_WRITER ".writeUint64" + funcPrefix + "(" + fieldName + ");";
                }
                else if (bType->isSignedInteger())
                {
                    if (width == 8)
                        return ATHENA_DNA_WRITER ".writeByte(" + fieldName + ");";
                    else if (width == 16)
                        return ATHENA_DNA_WRITER ".writeInt16" + funcPrefix + "(" + fieldName + ");";
                    else if (width == 32)
                        return ATHENA_DNA_WRITER ".writeInt32" + funcPrefix + "(" + fieldName + ");";
                    else if (width == 64)
                        return ATHENA_DNA_WRITER ".writeInt64" + funcPrefix + "(" + fieldName + ");";
                }
                else if (bType->isFloatingPoint())
                {
                    if (width == 32)
                        return ATHENA_DNA_WRITER ".writeFloat" + funcPrefix + "(" + fieldName + ");";
                    else if (width == 64)
                        return ATHENA_DNA_WRITER ".writeDouble" + funcPrefix + "(" + fieldName + ");";
                }
            }
            else if (theType->isRecordType())
            {
                const clang::CXXRecordDecl* rDecl = theType->getAsCXXRecordDecl();
                for (const clang::FieldDecl* field : rDecl->fields())
                {
                    if (!field->getName().compare("clangVec"))
                    {
                        const clang::VectorType* vType = (clang::VectorType*)field->getType().getTypePtr();
                        if (vType->isVectorType())
                        {
                            const clang::BuiltinType* eType = (clang::BuiltinType*)vType->getElementType().getTypePtr();
                            const uint64_t width = context.getTypeInfo(eType).Width;
                            if (!eType->isBuiltinType() || !eType->isFloatingPoint() ||
                                (width != 32 && width != 64))
                                continue;
                            if (vType->getNumElements() == 2)
                            {
                                if (width == 32)
                                    return ATHENA_DNA_WRITER ".writeVec2f" + funcPrefix + "(" + fieldName + ");";
                                else if (width == 64)
                                    return ATHENA_DNA_WRITER ".writeVec2d" + funcPrefix + "(" + fieldName + ");";
                            }
                            else if (vType->getNumElements() == 3)
                            {
                                if (width == 32)
                                    return ATHENA_DNA_WRITER ".writeVec3f" + funcPrefix + "(" + fieldName + ");";
                                else if (width == 64)
                                    return ATHENA_DNA_WRITER ".writeVec3d" + funcPrefix + "(" + fieldName + ");";
                            }
                            else if (vType->getNumElements() == 4)
                            {
                                if (width == 32)
                                    return ATHENA_DNA_WRITER ".writeVec4f" + funcPrefix + "(" + fieldName + ");";
                                else if (width == 64)
                                    return ATHENA_DNA_WRITER ".writeVec4d" + funcPrefix + "(" + fieldName + ");";
                            }
                        }
                    }
                }
                std::string baseDNA;
                bool isYAML = false;
                if (isDNARecord(rDecl, baseDNA, isYAML))
                {
                    isDNATypeOut = true;
                    return "write(" ATHENA_DNA_WRITER ");";
                }
            }
        }
        else
        {
            if (theType->isEnumeralType())
            {
                clang::EnumType* eType = (clang::EnumType*)theType;
                clang::EnumDecl* eDecl = eType->getDecl();
                theType = eDecl->getIntegerType().getCanonicalType().getTypePtr();
                std::string qName = eDecl->getQualifiedNameAsString();

                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isBooleanType())
                {
                    return qName + "(" ATHENA_DNA_READER ".readBool())";
                }
                else if (bType->isUnsignedInteger())
                {
                    if (width == 8)
                        return qName + "(" ATHENA_DNA_READER ".readUByte())";
                    else if (width == 16)
                        return qName + "(" ATHENA_DNA_READER ".readUint16" + funcPrefix + "())";
                    else if (width == 32)
                        return qName + "(" ATHENA_DNA_READER ".readUint32" + funcPrefix + "())";
                    else if (width == 64)
                        return qName + "(" ATHENA_DNA_READER ".readUint64" + funcPrefix + "())";
                }
                else if (bType->isSignedInteger())
                {
                    if (width == 8)
                        return qName + "(" ATHENA_DNA_READER ".readByte()";
                    else if (width == 16)
                        return qName + "(" ATHENA_DNA_READER ".readInt16" + funcPrefix + "())";
                    else if (width == 32)
                        return qName + "(" ATHENA_DNA_READER ".readInt32" + funcPrefix + "())";
                    else if (width == 64)
                        return qName + "(" ATHENA_DNA_READER ".readInt64" + funcPrefix + "())";
                }
            }
            else if (theType->isBuiltinType())
            {
                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isBooleanType())
                {
                    return ATHENA_DNA_READER ".readBool()";
                }
                else if (bType->isUnsignedInteger())
                {
                    if (width == 8)
                        return ATHENA_DNA_READER ".readUByte()";
                    else if (width == 16)
                        return ATHENA_DNA_READER ".readUint16" + funcPrefix + "()";
                    else if (width == 32)
                        return ATHENA_DNA_READER ".readUint32" + funcPrefix + "()";
                    else if (width == 64)
                        return ATHENA_DNA_READER ".readUint64" + funcPrefix + "()";
                }
                else if (bType->isSignedInteger())
                {
                    if (width == 8)
                        return ATHENA_DNA_READER ".readByte()";
                    else if (width == 16)
                        return ATHENA_DNA_READER ".readInt16" + funcPrefix + "()";
                    else if (width == 32)
                        return ATHENA_DNA_READER ".readInt32" + funcPrefix + "()";
                    else if (width == 64)
                        return ATHENA_DNA_READER ".readInt64" + funcPrefix + "()";
                }
                else if (bType->isFloatingPoint())
                {
                    if (width == 32)
                        return ATHENA_DNA_READER ".readFloat" + funcPrefix + "()";
                    else if (width == 64)
                        return ATHENA_DNA_READER ".readDouble" + funcPrefix + "()";
                }
            }
            else if (theType->isRecordType())
            {
                const clang::CXXRecordDecl* rDecl = theType->getAsCXXRecordDecl();
                for (const clang::FieldDecl* field : rDecl->fields())
                {
                    if (!field->getName().compare("clangVec"))
                    {
                        const clang::VectorType* vType = (clang::VectorType*)field->getType().getTypePtr();
                        if (vType->isVectorType())
                        {
                            const clang::BuiltinType* eType = (clang::BuiltinType*)vType->getElementType().getTypePtr();
                            const uint64_t width = context.getTypeInfo(eType).Width;
                            if (!eType->isBuiltinType() || !eType->isFloatingPoint() ||
                                (width != 32 && width != 64))
                                continue;
                            if (vType->getNumElements() == 2)
                            {
                                if (width == 32)
                                    return ATHENA_DNA_READER ".readVec2f" + funcPrefix + "()";
                                else if (width == 64)
                                    return ATHENA_DNA_READER ".readVec2d" + funcPrefix + "()";
                            }
                            else if (vType->getNumElements() == 3)
                            {
                                if (width == 32)
                                    return ATHENA_DNA_READER ".readVec3f" + funcPrefix + "()";
                                else if (width == 64)
                                    return ATHENA_DNA_READER ".readVec3d" + funcPrefix + "()";
                            }
                            else if (vType->getNumElements() == 4)
                            {
                                if (width == 32)
                                    return ATHENA_DNA_READER ".readVec4f" + funcPrefix + "()";
                                else if (width == 64)
                                    return ATHENA_DNA_READER ".readVec4d" + funcPrefix + "()";
                            }
                        }
                    }
                }
                std::string baseDNA;
                bool isYAML = false;
                if (isDNARecord(rDecl, baseDNA, isYAML))
                {
                    isDNATypeOut = true;
                    return "read(" ATHENA_DNA_READER ");";
                }
            }
        }
        return std::string();
    }

    std::string GetYAMLString(const clang::Type* theType, unsigned width,
                              const std::string& fieldName, const std::string& bareFieldName,
                              bool writerPass, bool& isDNATypeOut)
    {
        isDNATypeOut = false;
        if (writerPass)
        {
            if (theType->isEnumeralType())
            {
                clang::EnumType* eType = (clang::EnumType*)theType;
                clang::EnumDecl* eDecl = eType->getDecl();
                theType = eDecl->getIntegerType().getCanonicalType().getTypePtr();

                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isBooleanType())
                {
                    return ATHENA_YAML_WRITER ".writeBool(\"" + bareFieldName + "\", bool(" + fieldName + "));";
                }
                else if (bType->isUnsignedInteger())
                {
                    if (width == 8)
                        return ATHENA_YAML_WRITER ".writeUByte(\"" + bareFieldName + "\", atUint8(" + fieldName + "));";
                    else if (width == 16)
                        return ATHENA_YAML_WRITER ".writeUint16(\"" + bareFieldName + "\", atUint16(" + fieldName + "));";
                    else if (width == 32)
                        return ATHENA_YAML_WRITER ".writeUint32(\"" + bareFieldName + "\", atUint32(" + fieldName + "));";
                    else if (width == 64)
                        return ATHENA_YAML_WRITER ".writeUint64(\"" + bareFieldName + "\", atUint64(" + fieldName + "));";
                }
                else if (bType->isSignedInteger())
                {
                    if (width == 8)
                        return ATHENA_YAML_WRITER ".writeByte(\"" + bareFieldName + "\", atInt8(" + fieldName + "));";
                    else if (width == 16)
                        return ATHENA_YAML_WRITER ".writeInt16(\"" + bareFieldName + "\", atInt16(" + fieldName + "));";
                    else if (width == 32)
                        return ATHENA_YAML_WRITER ".writeInt32(\"" + bareFieldName + "\", atInt32(" + fieldName + "));";
                    else if (width == 64)
                        return ATHENA_YAML_WRITER ".writeInt64(\"" + bareFieldName + "\", atInt64(" + fieldName + "));";
                }
            }
            else if (theType->isBuiltinType())
            {
                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isBooleanType())
                {
                    return ATHENA_YAML_WRITER ".writeBool(\"" + bareFieldName + "\", " + fieldName + ");";
                }
                else if (bType->isUnsignedInteger())
                {
                    if (width == 8)
                        return ATHENA_YAML_WRITER ".writeUByte(\"" + bareFieldName + "\", " + fieldName + ");";
                    else if (width == 16)
                        return ATHENA_YAML_WRITER ".writeUint16(\"" + bareFieldName + "\", " + fieldName + ");";
                    else if (width == 32)
                        return ATHENA_YAML_WRITER ".writeUint32(\"" + bareFieldName + "\", " + fieldName + ");";
                    else if (width == 64)
                        return ATHENA_YAML_WRITER ".writeUint64(\"" + bareFieldName + "\", " + fieldName + ");";
                }
                else if (bType->isSignedInteger())
                {
                    if (width == 8)
                        return ATHENA_YAML_WRITER ".writeByte(\"" + bareFieldName + "\", " + fieldName + ");";
                    else if (width == 16)
                        return ATHENA_YAML_WRITER ".writeInt16(\"" + bareFieldName + "\", " + fieldName + ");";
                    else if (width == 32)
                        return ATHENA_YAML_WRITER ".writeInt32(\"" + bareFieldName + "\", " + fieldName + ");";
                    else if (width == 64)
                        return ATHENA_YAML_WRITER ".writeInt64(\"" + bareFieldName + "\", " + fieldName + ");";
                }
                else if (bType->isFloatingPoint())
                {
                    if (width == 32)
                        return ATHENA_YAML_WRITER ".writeFloat(\"" + bareFieldName + "\", " + fieldName + ");";
                    else if (width == 64)
                        return ATHENA_YAML_WRITER ".writeDouble(\"" + bareFieldName + "\", " + fieldName + ");";
                }
            }
            else if (theType->isRecordType())
            {
                const clang::CXXRecordDecl* rDecl = theType->getAsCXXRecordDecl();
                for (const clang::FieldDecl* field : rDecl->fields())
                {
                    if (!field->getName().compare("clangVec"))
                    {
                        const clang::VectorType* vType = (clang::VectorType*)field->getType().getTypePtr();
                        if (vType->isVectorType())
                        {
                            const clang::BuiltinType* eType = (clang::BuiltinType*)vType->getElementType().getTypePtr();
                            const uint64_t width = context.getTypeInfo(eType).Width;
                            if (!eType->isBuiltinType() || !eType->isFloatingPoint() ||
                                (width != 32 && width != 64))
                                continue;
                            if (vType->getNumElements() == 2)
                            {
                                if (width == 32)
                                    return ATHENA_YAML_WRITER ".writeVec2f(\"" + bareFieldName + "\", " + fieldName + ");";
                                else if (width == 64)
                                    return ATHENA_YAML_WRITER ".writeVec2d(\"" + bareFieldName + "\", " + fieldName + ");";
                            }
                            else if (vType->getNumElements() == 3)
                            {
                                if (width == 32)
                                    return ATHENA_YAML_WRITER ".writeVec3f(\"" + bareFieldName + "\", " + fieldName + ");";
                                else if (width == 64)
                                    return ATHENA_YAML_WRITER ".writeVec3d(\"" + bareFieldName + "\", " + fieldName + ");";
                            }
                            else if (vType->getNumElements() == 4)
                            {
                                if (width == 32)
                                    return ATHENA_YAML_WRITER ".writeVec4f(\"" + bareFieldName + "\", " + fieldName + ");";
                                else if (width == 64)
                                    return ATHENA_YAML_WRITER ".writeVec4d(\"" + bareFieldName + "\", " + fieldName + ");";
                            }
                        }
                    }
                }
                std::string baseDNA;
                bool isYAML = false;
                if (isDNARecord(rDecl, baseDNA, isYAML))
                {
                    isDNATypeOut = true;
                    return "write(" ATHENA_YAML_WRITER ");";
                }
            }
        }
        else
        {
            if (theType->isEnumeralType())
            {
                clang::EnumType* eType = (clang::EnumType*)theType;
                clang::EnumDecl* eDecl = eType->getDecl();
                theType = eDecl->getIntegerType().getCanonicalType().getTypePtr();
                std::string qName = eDecl->getQualifiedNameAsString();

                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isBooleanType())
                {
                    return qName + "(" ATHENA_YAML_READER ".readBool(\"" + bareFieldName + "\"))";
                }
                else if (bType->isUnsignedInteger())
                {
                    if (width == 8)
                        return qName + "(" ATHENA_YAML_READER ".readUByte(\"" + bareFieldName + "\"))";
                    else if (width == 16)
                        return qName + "(" ATHENA_YAML_READER ".readUint16(\"" + bareFieldName + "\"))";
                    else if (width == 32)
                        return qName + "(" ATHENA_YAML_READER ".readUint32(\"" + bareFieldName + "\"))";
                    else if (width == 64)
                        return qName + "(" ATHENA_YAML_READER ".readUint64(\"" + bareFieldName + "\"))";
                }
                else if (bType->isSignedInteger())
                {
                    if (width == 8)
                        return qName + "(" ATHENA_YAML_READER ".readByte(\"" + bareFieldName + "\"))";
                    else if (width == 16)
                        return qName + "(" ATHENA_YAML_READER ".readInt16(\"" + bareFieldName + "\"))";
                    else if (width == 32)
                        return qName + "(" ATHENA_YAML_READER ".readInt32(\"" + bareFieldName + "\"))";
                    else if (width == 64)
                        return qName + "(" ATHENA_YAML_READER ".readInt64(\"" + bareFieldName + "\"))";
                }
            }
            else if (theType->isBuiltinType())
            {
                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isBooleanType())
                {
                    return ATHENA_YAML_READER ".readBool(\"" + bareFieldName + "\")";
                }
                else if (bType->isUnsignedInteger())
                {
                    if (width == 8)
                        return ATHENA_YAML_READER ".readUByte(\"" + bareFieldName + "\")";
                    else if (width == 16)
                        return ATHENA_YAML_READER ".readUint16(\"" + bareFieldName + "\")";
                    else if (width == 32)
                        return ATHENA_YAML_READER ".readUint32(\"" + bareFieldName + "\")";
                    else if (width == 64)
                        return ATHENA_YAML_READER ".readUint64(\"" + bareFieldName + "\")";
                }
                else if (bType->isSignedInteger())
                {
                    if (width == 8)
                        return ATHENA_YAML_READER ".readByte(\"" + bareFieldName + "\")";
                    else if (width == 16)
                        return ATHENA_YAML_READER ".readInt16(\"" + bareFieldName + "\")";
                    else if (width == 32)
                        return ATHENA_YAML_READER ".readInt32(\"" + bareFieldName + "\")";
                    else if (width == 64)
                        return ATHENA_YAML_READER ".readInt64(\"" + bareFieldName + "\")";
                }
                else if (bType->isFloatingPoint())
                {
                    if (width == 32)
                        return ATHENA_YAML_READER ".readFloat(\"" + bareFieldName + "\")";
                    else if (width == 64)
                        return ATHENA_YAML_READER ".readDouble(\"" + bareFieldName + "\")";
                }
            }
            else if (theType->isRecordType())
            {
                const clang::CXXRecordDecl* rDecl = theType->getAsCXXRecordDecl();
                for (const clang::FieldDecl* field : rDecl->fields())
                {
                    if (!field->getName().compare("clangVec"))
                    {
                        const clang::VectorType* vType = (clang::VectorType*)field->getType().getTypePtr();
                        if (vType->isVectorType())
                        {
                            const clang::BuiltinType* eType = (clang::BuiltinType*)vType->getElementType().getTypePtr();
                            const uint64_t width = context.getTypeInfo(eType).Width;
                            if (!eType->isBuiltinType() || !eType->isFloatingPoint() ||
                                (width != 32 && width != 64))
                                continue;
                            if (vType->getNumElements() == 2)
                            {
                                if (width == 32)
                                    return ATHENA_YAML_READER ".readVec2f(\"" + bareFieldName + "\")";
                                else if (width == 64)
                                    return ATHENA_YAML_READER ".readVec2d(\"" + bareFieldName + "\")";
                            }
                            else if (vType->getNumElements() == 3)
                            {
                                if (width == 32)
                                    return ATHENA_YAML_READER ".readVec3f(\"" + bareFieldName + "\")";
                                else if (width == 64)
                                    return ATHENA_YAML_READER ".readVec3d(\"" + bareFieldName + "\")";
                            }
                            else if (vType->getNumElements() == 4)
                            {
                                if (width == 32)
                                    return ATHENA_YAML_READER ".readVec4f(\"" + bareFieldName + "\")";
                                else if (width == 64)
                                    return ATHENA_YAML_READER ".readVec4d(\"" + bareFieldName + "\")";
                            }
                        }
                    }
                }
                std::string baseDNA;
                bool isYAML = false;
                if (isDNARecord(rDecl, baseDNA, isYAML))
                {
                    isDNATypeOut = true;
                    return "read(" ATHENA_YAML_READER ");";
                }
            }
        }
        return std::string();
    }
#endif

    static std::string GetFieldString(const std::string& fieldName)
    {
        size_t underscorePos = fieldName.find('_');
        std::string idString = fieldName;
        if (underscorePos != std::string::npos && underscorePos != fieldName.size() - 1)
            idString.assign(fieldName.begin() + underscorePos + 1, fieldName.end());
        return idString;
    }

    static std::string GetPropIdExpr(const clang::FieldDecl* field, const std::string& fieldName)
    {
        std::string fieldStr = GetFieldString(fieldName);
        std::string propIdExpr = "\"" + fieldStr + "\"";
        for (clang::Attr* attr : field->attrs())
        {
            if (clang::AnnotateAttr* annot = clang::dyn_cast<clang::AnnotateAttr>(attr))
            {
                llvm::StringRef text_ref = annot->getAnnotation();
                if (text_ref.startswith_lower("rcrc32="))
                {
                    unsigned long num = strtoul(text_ref.data() + 7, nullptr, 16);
                    std::string tmpS;
                    llvm::raw_string_ostream s(tmpS);
                    s << llvm::format("\"%s\", 0x%08X", fieldStr.c_str(), num);
                    propIdExpr = s.str();
                    break;
                }
            }
        }
        return propIdExpr;
    }

    static std::string GetOpString(const std::string& fieldName, const std::string& propIdExpr, int64_t endianVal)
    {

        return "Do<Op, "s + (endianVal ? "Endian::Big" : "Endian::Little") + ">({" + propIdExpr + "}, " + fieldName + ", s)";
    }

    static std::string GetOpString(const std::string& fieldName, const std::string& propIdExpr)
    {

        return "Do<Op>({" + propIdExpr + "}, " + fieldName + ", s)";
    }

    static std::string GetVectorOpString(const std::string& fieldName, const std::string& propIdExpr, const std::string& sizeExpr, int64_t endianVal)
    {
        return "Do<Op, "s + (endianVal ? "Endian::Big" : "Endian::Little") + ">({" + propIdExpr + "}, " + fieldName + ", " + sizeExpr + ", s)";
    }

    static std::string GetVectorOpString(const std::string& fieldName, const std::string& propIdExpr, const std::string& sizeExpr)
    {
        return "Do<Op>({" + propIdExpr + "}, " + fieldName + ", " + sizeExpr + ", s)";
    }

    void emitEnumerateFunc(clang::CXXRecordDecl* decl, const std::string& baseDNA)
    {
        fileOut << "template <class Op>\nvoid " <<
            decl->getQualifiedNameAsString() << "::Enumerate(typename Op::StreamT& s)\n{\n";

        if (baseDNA.size())
            fileOut << "    " << baseDNA << "::Enumerate<Op>(s);\n";

        for (const clang::FieldDecl* field : decl->fields())
        {
            clang::QualType qualType = field->getType();
            const clang::Type* regType = qualType.getTypePtrOrNull();
            if (!regType || regType->getTypeClass() == clang::Type::TemplateTypeParm)
                continue;
            clang::TypeInfo regTypeInfo = context.getTypeInfo(qualType);
            while (regType->getTypeClass() == clang::Type::Elaborated ||
                   regType->getTypeClass() == clang::Type::Typedef)
                regType = regType->getUnqualifiedDesugaredType();

            /* Resolve constant array */
            size_t arraySize = 1;
            bool isArray = false;
            if (regType->getTypeClass() == clang::Type::ConstantArray)
            {
                isArray = true;
                const clang::ConstantArrayType* caType = (clang::ConstantArrayType*)regType;
                arraySize = caType->getSize().getZExtValue();
                qualType = caType->getElementType();
                regTypeInfo = context.getTypeInfo(qualType);
                regType = qualType.getTypePtrOrNull();
                if (regType->getTypeClass() == clang::Type::Elaborated)
                    regType = regType->getUnqualifiedDesugaredType();
            }

            for (int e=0 ; e<arraySize ; ++e)
            {
                std::string fieldName;
                if (isArray)
                {
                    char subscript[16];
                    snprintf(subscript, 16, "[%d]", e);
                    fieldName = field->getName().str() + subscript;
                }
                else
                    fieldName = field->getName();

                std::string propIdExpr = GetPropIdExpr(field, fieldName);

                if (regType->getTypeClass() == clang::Type::TemplateSpecialization)
                {
                    const clang::TemplateSpecializationType* tsType = (const clang::TemplateSpecializationType*)regType;
                    const clang::TemplateDecl* tsDecl = tsType->getTemplateName().getAsTemplateDecl();
                    const clang::TemplateParameterList* classParms = tsDecl->getTemplateParameters();

                    if (!tsDecl->getName().compare("Value"))
                    {
                        llvm::APSInt endian(64, -1);
                        const clang::Expr* endianExpr = nullptr;
                        if (classParms->size() >= 2)
                        {
                            const clang::NamedDecl* endianParm = classParms->getParam(1);
                            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm)
                            {
                                const clang::NonTypeTemplateParmDecl* nttParm = (clang::NonTypeTemplateParmDecl*)endianParm;
                                const clang::Expr* defArg = nttParm->getDefaultArgument();
                                endianExpr = defArg;
                                if (!defArg->isIntegerConstantExpr(endian, context))
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(defArg->getLocStart(), AthenaError);
                                    diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                    diag.AddSourceRange(clang::CharSourceRange(defArg->getSourceRange(), true));
                                    continue;
                                }
                            }
                        }

                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::Expr* expr = arg.getAsExpr();
                                endianExpr = expr;
                                if (!expr->isIntegerConstantExpr(endian, context))
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                    diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                    diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                    continue;
                                }
                            }
                        }

                        int64_t endianVal = endian.getSExtValue();
                        if (endianVal != 0 && endianVal != 1)
                        {
                            if (endianExpr)
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(endianExpr->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(endianExpr->getSourceRange(), true));
                            }
                            else
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            }
                            continue;
                        }

                        clang::QualType templateType;
                        std::string ioOp;
                        bool isDNAType = false;
                        const clang::TemplateArgument* typeArg = nullptr;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Type)
                            {
                                typeArg = &arg;
                                templateType = arg.getAsType().getCanonicalType();
                                ioOp = GetOpString(fieldName, propIdExpr, endianVal);
                            }
                        }

                        if (ioOp.empty())
                        {
                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                            diag.AddString("Unable to use type '" + tsDecl->getName().str() + "' with Athena");
                            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            continue;
                        }

                        fileOut << "    " << ioOp << ";\n";
                    }
                    else if (!tsDecl->getName().compare("Vector"))
                    {
                        llvm::APSInt endian(64, -1);
                        const clang::Expr* endianExpr = nullptr;
                        if (classParms->size() >= 3)
                        {
                            const clang::NamedDecl* endianParm = classParms->getParam(2);
                            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm)
                            {
                                const clang::NonTypeTemplateParmDecl* nttParm = (clang::NonTypeTemplateParmDecl*)endianParm;
                                const clang::Expr* defArg = nttParm->getDefaultArgument();
                                endianExpr = defArg;
                                if (!defArg->isIntegerConstantExpr(endian, context))
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(defArg->getLocStart(), AthenaError);
                                    diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                    diag.AddSourceRange(clang::CharSourceRange(defArg->getSourceRange(), true));
                                    continue;
                                }
                            }
                        }

                        std::string sizeExpr;
                        const clang::TemplateArgument* sizeArg = nullptr;
                        size_t idx = 0;
                        bool bad = false;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
                                if (idx == 1)
                                {
                                    sizeArg = &arg;
                                    const clang::UnaryExprOrTypeTraitExpr* uExpr = (clang::UnaryExprOrTypeTraitExpr*)expr;
                                    if (uExpr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                                        uExpr->getKind() == clang::UETT_SizeOf)
                                    {
                                        const clang::Expr* argExpr = uExpr->getArgumentExpr();
                                        while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                                            argExpr = ((clang::ParenExpr*)argExpr)->getSubExpr();
                                        llvm::raw_string_ostream strStream(sizeExpr);
                                        argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                                    }
                                }
                                else if (idx == 2)
                                {
                                    endianExpr = expr;
                                    if (!expr->isIntegerConstantExpr(endian, context))
                                    {
                                        clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                        diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                        diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                        bad = true;
                                        break;
                                    }
                                }
                            }
                            ++idx;
                        }
                        if (bad)
                            continue;

                        int64_t endianVal = endian.getSExtValue();
                        if (endianVal != 0 && endianVal != 1)
                        {
                            if (endianExpr)
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(endianExpr->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(endianExpr->getSourceRange(), true));
                            }
                            else
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            }
                            continue;
                        }

                        clang::QualType templateType;
                        std::string ioOp;
                        bool isDNAType = false;
                        const clang::TemplateArgument* typeArg = nullptr;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Type)
                            {
                                typeArg = &arg;
                                templateType = arg.getAsType().getCanonicalType();
                                ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExpr, endianVal);
                            }
                        }

                        if (ioOp.empty())
                        {
                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                            diag.AddString("Unable to use type '" + templateType.getAsString() + "' with Athena");
                            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            continue;
                        }

                        if (sizeExpr.empty())
                        {
                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                            diag.AddString("Unable to use count variable with Athena");
                            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            continue;
                        }

                        fileOut << "    " << ioOp << ";\n";

                    }
                    else if (!tsDecl->getName().compare("Buffer"))
                    {
                        const clang::Expr* sizeExpr = nullptr;
                        std::string sizeExprStr;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::UnaryExprOrTypeTraitExpr* uExpr = (clang::UnaryExprOrTypeTraitExpr*)arg.getAsExpr()->IgnoreImpCasts();
                                if (uExpr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                                    uExpr->getKind() == clang::UETT_SizeOf)
                                {
                                    const clang::Expr* argExpr = uExpr->getArgumentExpr();
                                    while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                                        argExpr = ((clang::ParenExpr*)argExpr)->getSubExpr();
                                    sizeExpr = argExpr;
                                    llvm::raw_string_ostream strStream(sizeExprStr);
                                    argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                                }
                            }
                        }
                        if (sizeExprStr.empty())
                        {
                            if (sizeExpr)
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(sizeExpr->getLocStart(), AthenaError);
                                diag.AddString("Unable to use size variable with Athena");
                                diag.AddSourceRange(clang::CharSourceRange(sizeExpr->getSourceRange(), true));
                            }
                            else
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                diag.AddString("Unable to use size variable with Athena");
                                diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            }
                            continue;
                        }

                        std::string ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr);

                        fileOut << "    " << ioOp << ";\n";
                    }
                    else if (!tsDecl->getName().compare("String"))
                    {
                        const clang::Expr* sizeExpr = nullptr;
                        std::string sizeExprStr;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
                                const clang::UnaryExprOrTypeTraitExpr* uExpr = (clang::UnaryExprOrTypeTraitExpr*)expr;
                                llvm::APSInt sizeLiteral;
                                if (expr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                                    uExpr->getKind() == clang::UETT_SizeOf)
                                {
                                    const clang::Expr* argExpr = uExpr->getArgumentExpr();
                                    while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                                        argExpr = ((clang::ParenExpr*)argExpr)->getSubExpr();
                                    sizeExpr = argExpr;
                                    llvm::raw_string_ostream strStream(sizeExprStr);
                                    argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                                }
                                else if (expr->isIntegerConstantExpr(sizeLiteral, context))
                                {
                                    sizeExprStr = sizeLiteral.toString(10);
                                }
                            }
                        }

                        std::string ioOp;
                        if (!sizeExprStr.empty())
                            ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr);
                        else
                            ioOp = GetOpString(fieldName, propIdExpr);

                        fileOut << "    " << ioOp << ";\n";
                    }
                    else if (!tsDecl->getName().compare("WString"))
                    {
                        llvm::APSInt endian(64, -1);
                        const clang::Expr* endianExpr = nullptr;
                        if (classParms->size() >= 2)
                        {
                            const clang::NamedDecl* endianParm = classParms->getParam(1);
                            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm)
                            {
                                const clang::NonTypeTemplateParmDecl* nttParm = (clang::NonTypeTemplateParmDecl*)endianParm;
                                const clang::Expr* defArg = nttParm->getDefaultArgument();
                                endianExpr = defArg;
                                if (!defArg->isIntegerConstantExpr(endian, context))
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(defArg->getLocStart(), AthenaError);
                                    diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                    diag.AddSourceRange(clang::CharSourceRange(defArg->getSourceRange(), true));
                                    continue;
                                }
                            }
                        }

                        const clang::Expr* sizeExpr = nullptr;
                        std::string sizeExprStr;
                        size_t idx = 0;
                        bool bad = false;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
                                if (idx == 0)
                                {
                                    llvm::APSInt sizeLiteral;
                                    const clang::UnaryExprOrTypeTraitExpr* uExpr = (clang::UnaryExprOrTypeTraitExpr*)expr;
                                    if (expr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                                        uExpr->getKind() == clang::UETT_SizeOf)
                                    {
                                        const clang::Expr* argExpr = uExpr->getArgumentExpr();
                                        while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                                            argExpr = ((clang::ParenExpr*)argExpr)->getSubExpr();
                                        sizeExpr = argExpr;
                                        llvm::raw_string_ostream strStream(sizeExprStr);
                                        argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                                    }
                                    else if (expr->isIntegerConstantExpr(sizeLiteral, context))
                                    {
                                        sizeExprStr = sizeLiteral.toString(10);
                                    }
                                }
                                else if (idx == 1)
                                {
                                    endianExpr = expr;
                                    if (!expr->isIntegerConstantExpr(endian, context))
                                    {
                                        clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                        diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                        diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                        bad = true;
                                        break;
                                    }
                                }
                            }
                            ++idx;
                        }
                        if (bad)
                            continue;

                        int64_t endianVal = endian.getSExtValue();
                        if (endianVal != 0 && endianVal != 1)
                        {
                            if (endianExpr)
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(endianExpr->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(endianExpr->getSourceRange(), true));
                            }
                            else
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            }
                            continue;
                        }

                        std::string ioOp;
                        if (!sizeExprStr.empty())
                            ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr, endianVal);
                        else
                            ioOp = GetOpString(fieldName, propIdExpr, endianVal);

                        fileOut << "    " << ioOp << ";\n";
                    }
                    else if (!tsDecl->getName().compare("Seek"))
                    {
                        size_t idx = 0;
                        const clang::Expr* offsetExpr = nullptr;
                        std::string offsetExprStr;
                        llvm::APSInt direction(64, 0);
                        const clang::Expr* directionExpr = nullptr;
                        bool bad = false;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
                                if (!idx)
                                {
                                    offsetExpr = expr;
                                    const clang::UnaryExprOrTypeTraitExpr* uExpr = (clang::UnaryExprOrTypeTraitExpr*)expr;
                                    llvm::APSInt offsetLiteral;
                                    if (expr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                                        uExpr->getKind() == clang::UETT_SizeOf)
                                    {
                                        const clang::Expr* argExpr = uExpr->getArgumentExpr();
                                        while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                                            argExpr = ((clang::ParenExpr*)argExpr)->getSubExpr();
                                        offsetExpr = argExpr;
                                        llvm::raw_string_ostream strStream(offsetExprStr);
                                        argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                                    }
                                    else if (expr->isIntegerConstantExpr(offsetLiteral, context))
                                    {
                                        offsetExprStr = offsetLiteral.toString(10);
                                    }
                                }
                                else
                                {
                                    directionExpr = expr;
                                    if (!expr->isIntegerConstantExpr(direction, context))
                                    {
                                        clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                        diag.AddString("Unable to use non-constant direction expression in Athena");
                                        diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                        bad = true;
                                        break;
                                    }
                                }
                            }
                            ++idx;
                        }
                        if (bad)
                            continue;

                        int64_t directionVal = direction.getSExtValue();
                        if (directionVal < 0 || directionVal > 2)
                        {
                            if (directionExpr)
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(directionExpr->getLocStart(), AthenaError);
                                diag.AddString("Direction parameter must be 'Begin', 'Current', or 'End'");
                                diag.AddSourceRange(clang::CharSourceRange(directionExpr->getSourceRange(), true));
                            }
                            else
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                diag.AddString("Direction parameter must be 'Begin', 'Current', or 'End'");
                                diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            }
                            continue;
                        }

                        if (directionVal == 0)
                            fileOut << "    DoSeek<Op>(" << offsetExprStr << ", Begin, s);\n";
                        else if (directionVal == 1)
                            fileOut << "    DoSeek<Op>(" << offsetExprStr << ", Current, s);\n";
                        else if (directionVal == 2)
                            fileOut << "    DoSeek<Op>(" << offsetExprStr << ", End, s);\n";

                    }
                    else if (!tsDecl->getName().compare("Align"))
                    {
                        llvm::APSInt align(64, 0);
                        bool bad = false;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::Expr* expr = arg.getAsExpr();
                                if (!expr->isIntegerConstantExpr(align, context))
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                    diag.AddString("Unable to use non-constant align expression in Athena");
                                    diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                    bad = true;
                                    break;
                                }
                            }
                        }
                        if (bad)
                            continue;

                        int64_t alignVal = align.getSExtValue();
                        if (alignVal)
                        {
                            fileOut << "    DoAlign<Op>(" << alignVal << ", s);\n";
                        }
                    }

                }

                else if (regType->getTypeClass() == clang::Type::Record)
                {
                    const clang::CXXRecordDecl* cxxRDecl = regType->getAsCXXRecordDecl();
                    std::string baseDNA;
                    if (cxxRDecl && isDNARecord(cxxRDecl, baseDNA))
                        fileOut << "    " << GetOpString(fieldName, propIdExpr) << ";\n";
                }

            }

        }

        fileOut << "}\n\n";
    }

    void emitLookupFunc(clang::CXXRecordDecl* decl, const std::string& baseDNA)
    {
        fileOut << "template <class Op>\nbool " <<
                decl->getQualifiedNameAsString() << "::Lookup(uint64_t hash, typename Op::StreamT& s)\n{\n";

        if (baseDNA.size())
            fileOut << "    if (" << baseDNA << "::Lookup<Op>(hash, s))\n"
                    << "        return true;\n";

        fileOut << "    switch (hash)\n"
                << "    {\n";

        for (const clang::FieldDecl* field : decl->fields())
        {
            clang::QualType qualType = field->getType();
            const clang::Type* regType = qualType.getTypePtrOrNull();
            if (!regType || regType->getTypeClass() == clang::Type::TemplateTypeParm)
                continue;
            clang::TypeInfo regTypeInfo = context.getTypeInfo(qualType);
            while (regType->getTypeClass() == clang::Type::Elaborated ||
                   regType->getTypeClass() == clang::Type::Typedef)
                regType = regType->getUnqualifiedDesugaredType();

            /* Resolve constant array */
            size_t arraySize = 1;
            bool isArray = false;
            if (regType->getTypeClass() == clang::Type::ConstantArray)
            {
                isArray = true;
                const clang::ConstantArrayType* caType = (clang::ConstantArrayType*)regType;
                arraySize = caType->getSize().getZExtValue();
                qualType = caType->getElementType();
                regTypeInfo = context.getTypeInfo(qualType);
                regType = qualType.getTypePtrOrNull();
                if (regType->getTypeClass() == clang::Type::Elaborated)
                    regType = regType->getUnqualifiedDesugaredType();
            }

            for (int e=0 ; e<arraySize ; ++e)
            {
                std::string fieldName;
                if (isArray)
                {
                    char subscript[16];
                    snprintf(subscript, 16, "[%d]", e);
                    fieldName = field->getName().str() + subscript;
                }
                else
                    fieldName = field->getName();

                std::string propIdExpr = GetPropIdExpr(field, fieldName);

                if (regType->getTypeClass() == clang::Type::TemplateSpecialization)
                {
                    const clang::TemplateSpecializationType* tsType = (const clang::TemplateSpecializationType*)regType;
                    const clang::TemplateDecl* tsDecl = tsType->getTemplateName().getAsTemplateDecl();
                    const clang::TemplateParameterList* classParms = tsDecl->getTemplateParameters();

                    if (!tsDecl->getName().compare("Value"))
                    {
                        llvm::APSInt endian(64, -1);
                        const clang::Expr* endianExpr = nullptr;
                        if (classParms->size() >= 2)
                        {
                            const clang::NamedDecl* endianParm = classParms->getParam(1);
                            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm)
                            {
                                const clang::NonTypeTemplateParmDecl* nttParm = (clang::NonTypeTemplateParmDecl*)endianParm;
                                const clang::Expr* defArg = nttParm->getDefaultArgument();
                                endianExpr = defArg;
                                if (!defArg->isIntegerConstantExpr(endian, context))
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(defArg->getLocStart(), AthenaError);
                                    diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                    diag.AddSourceRange(clang::CharSourceRange(defArg->getSourceRange(), true));
                                    continue;
                                }
                            }
                        }

                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::Expr* expr = arg.getAsExpr();
                                endianExpr = expr;
                                if (!expr->isIntegerConstantExpr(endian, context))
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                    diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                    diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                    continue;
                                }
                            }
                        }

                        int64_t endianVal = endian.getSExtValue();
                        if (endianVal != 0 && endianVal != 1)
                        {
                            if (endianExpr)
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(endianExpr->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(endianExpr->getSourceRange(), true));
                            }
                            else
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            }
                            continue;
                        }

                        clang::QualType templateType;
                        std::string ioOp;
                        bool isDNAType = false;
                        const clang::TemplateArgument* typeArg = nullptr;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Type)
                            {
                                typeArg = &arg;
                                templateType = arg.getAsType().getCanonicalType();
                                ioOp = GetOpString(fieldName, propIdExpr, endianVal);
                            }
                        }

                        if (ioOp.empty())
                        {
                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                            diag.AddString("Unable to use type '" + tsDecl->getName().str() + "' with Athena");
                            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            continue;
                        }

                        fileOut << "    AT_PROP_CASE(" << propIdExpr << "):\n"
                                << "        " << ioOp << ";\n"
                                << "        return true;\n";

                    }
                    else if (!tsDecl->getName().compare("Vector"))
                    {
                        llvm::APSInt endian(64, -1);
                        const clang::Expr* endianExpr = nullptr;
                        if (classParms->size() >= 3)
                        {
                            const clang::NamedDecl* endianParm = classParms->getParam(2);
                            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm)
                            {
                                const clang::NonTypeTemplateParmDecl* nttParm = (clang::NonTypeTemplateParmDecl*)endianParm;
                                const clang::Expr* defArg = nttParm->getDefaultArgument();
                                endianExpr = defArg;
                                if (!defArg->isIntegerConstantExpr(endian, context))
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(defArg->getLocStart(), AthenaError);
                                    diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                    diag.AddSourceRange(clang::CharSourceRange(defArg->getSourceRange(), true));
                                    continue;
                                }
                            }
                        }

                        std::string sizeExpr;
                        const clang::TemplateArgument* sizeArg = nullptr;
                        size_t idx = 0;
                        bool bad = false;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
                                if (idx == 1)
                                {
                                    sizeArg = &arg;
                                    const clang::UnaryExprOrTypeTraitExpr* uExpr = (clang::UnaryExprOrTypeTraitExpr*)expr;
                                    if (uExpr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                                        uExpr->getKind() == clang::UETT_SizeOf)
                                    {
                                        const clang::Expr* argExpr = uExpr->getArgumentExpr();
                                        while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                                            argExpr = ((clang::ParenExpr*)argExpr)->getSubExpr();
                                        llvm::raw_string_ostream strStream(sizeExpr);
                                        argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                                    }
                                }
                                else if (idx == 2)
                                {
                                    endianExpr = expr;
                                    if (!expr->isIntegerConstantExpr(endian, context))
                                    {
                                        clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                        diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                        diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                        bad = true;
                                        break;
                                    }
                                }
                            }
                            ++idx;
                        }
                        if (bad)
                            continue;

                        int64_t endianVal = endian.getSExtValue();
                        if (endianVal != 0 && endianVal != 1)
                        {
                            if (endianExpr)
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(endianExpr->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(endianExpr->getSourceRange(), true));
                            }
                            else
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            }
                            continue;
                        }

                        clang::QualType templateType;
                        std::string ioOp;
                        bool isDNAType = false;
                        const clang::TemplateArgument* typeArg = nullptr;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Type)
                            {
                                typeArg = &arg;
                                templateType = arg.getAsType().getCanonicalType();
                                ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExpr, endianVal);
                            }
                        }

                        if (ioOp.empty())
                        {
                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                            diag.AddString("Unable to use type '" + templateType.getAsString() + "' with Athena");
                            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            continue;
                        }

                        if (sizeExpr.empty())
                        {
                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                            diag.AddString("Unable to use count variable with Athena");
                            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            continue;
                        }

                        fileOut << "    AT_PROP_CASE(" << propIdExpr << "):\n"
                                << "        " << ioOp << ";\n"
                                << "        return true;\n";
                    }
                    else if (!tsDecl->getName().compare("Buffer"))
                    {
                        const clang::Expr* sizeExpr = nullptr;
                        std::string sizeExprStr;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::UnaryExprOrTypeTraitExpr* uExpr = (clang::UnaryExprOrTypeTraitExpr*)arg.getAsExpr()->IgnoreImpCasts();
                                if (uExpr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                                    uExpr->getKind() == clang::UETT_SizeOf)
                                {
                                    const clang::Expr* argExpr = uExpr->getArgumentExpr();
                                    while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                                        argExpr = ((clang::ParenExpr*)argExpr)->getSubExpr();
                                    sizeExpr = argExpr;
                                    llvm::raw_string_ostream strStream(sizeExprStr);
                                    argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                                }
                            }
                        }
                        if (sizeExprStr.empty())
                        {
                            if (sizeExpr)
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(sizeExpr->getLocStart(), AthenaError);
                                diag.AddString("Unable to use size variable with Athena");
                                diag.AddSourceRange(clang::CharSourceRange(sizeExpr->getSourceRange(), true));
                            }
                            else
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                diag.AddString("Unable to use size variable with Athena");
                                diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            }
                            continue;
                        }

                        std::string ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr);

                        fileOut << "    AT_PROP_CASE(" << propIdExpr << "):\n"
                                << "        " << ioOp << ";\n"
                                << "        return true;\n";
                    }
                    else if (!tsDecl->getName().compare("String"))
                    {
                        const clang::Expr* sizeExpr = nullptr;
                        std::string sizeExprStr;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
                                const clang::UnaryExprOrTypeTraitExpr* uExpr = (clang::UnaryExprOrTypeTraitExpr*)expr;
                                llvm::APSInt sizeLiteral;
                                if (expr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                                    uExpr->getKind() == clang::UETT_SizeOf)
                                {
                                    const clang::Expr* argExpr = uExpr->getArgumentExpr();
                                    while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                                        argExpr = ((clang::ParenExpr*)argExpr)->getSubExpr();
                                    sizeExpr = argExpr;
                                    llvm::raw_string_ostream strStream(sizeExprStr);
                                    argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                                }
                                else if (expr->isIntegerConstantExpr(sizeLiteral, context))
                                {
                                    sizeExprStr = sizeLiteral.toString(10);
                                }
                            }
                        }

                        std::string ioOp;
                        if (!sizeExprStr.empty())
                            ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr);
                        else
                            ioOp = GetOpString(fieldName, propIdExpr);

                        fileOut << "    AT_PROP_CASE(" << propIdExpr << "):\n"
                                << "        " << ioOp << ";\n"
                                << "        return true;\n";
                    }
                    else if (!tsDecl->getName().compare("WString"))
                    {
                        llvm::APSInt endian(64, -1);
                        const clang::Expr* endianExpr = nullptr;
                        if (classParms->size() >= 2)
                        {
                            const clang::NamedDecl* endianParm = classParms->getParam(1);
                            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm)
                            {
                                const clang::NonTypeTemplateParmDecl* nttParm = (clang::NonTypeTemplateParmDecl*)endianParm;
                                const clang::Expr* defArg = nttParm->getDefaultArgument();
                                endianExpr = defArg;
                                if (!defArg->isIntegerConstantExpr(endian, context))
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(defArg->getLocStart(), AthenaError);
                                    diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                    diag.AddSourceRange(clang::CharSourceRange(defArg->getSourceRange(), true));
                                    continue;
                                }
                            }
                        }

                        const clang::Expr* sizeExpr = nullptr;
                        std::string sizeExprStr;
                        size_t idx = 0;
                        bool bad = false;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
                                if (idx == 0)
                                {
                                    llvm::APSInt sizeLiteral;
                                    const clang::UnaryExprOrTypeTraitExpr* uExpr = (clang::UnaryExprOrTypeTraitExpr*)expr;
                                    if (expr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                                        uExpr->getKind() == clang::UETT_SizeOf)
                                    {
                                        const clang::Expr* argExpr = uExpr->getArgumentExpr();
                                        while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                                            argExpr = ((clang::ParenExpr*)argExpr)->getSubExpr();
                                        sizeExpr = argExpr;
                                        llvm::raw_string_ostream strStream(sizeExprStr);
                                        argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                                    }
                                    else if (expr->isIntegerConstantExpr(sizeLiteral, context))
                                    {
                                        sizeExprStr = sizeLiteral.toString(10);
                                    }
                                }
                                else if (idx == 1)
                                {
                                    endianExpr = expr;
                                    if (!expr->isIntegerConstantExpr(endian, context))
                                    {
                                        clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                        diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                        diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                        bad = true;
                                        break;
                                    }
                                }
                            }
                            ++idx;
                        }
                        if (bad)
                            continue;

                        int64_t endianVal = endian.getSExtValue();
                        if (endianVal != 0 && endianVal != 1)
                        {
                            if (endianExpr)
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(endianExpr->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(endianExpr->getSourceRange(), true));
                            }
                            else
                            {
                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                            }
                            continue;
                        }

                        std::string ioOp;
                        if (!sizeExprStr.empty())
                            ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr, endianVal);
                        else
                            ioOp = GetOpString(fieldName, propIdExpr, endianVal);

                        fileOut << "    AT_PROP_CASE(" << propIdExpr << "):\n"
                                << "        " << ioOp << ";\n"
                                << "        return true;\n";
                    }
                }

                else if (regType->getTypeClass() == clang::Type::Record)
                {
                    const clang::CXXRecordDecl* cxxRDecl = regType->getAsCXXRecordDecl();
                    std::string baseDNA;
                    if (cxxRDecl && isDNARecord(cxxRDecl, baseDNA))
                    {
                        fileOut << "    AT_PROP_CASE(" << propIdExpr << "):\n"
                                << "        " << GetOpString(fieldName, propIdExpr) << ";\n"
                                << "        return true;\n";
                    }
                }

            }

        }

        fileOut << "    default:\n        return false;\n    }\n}\n\n";
    }

public:
    explicit ATDNAEmitVisitor(clang::ASTContext& ctxin, StreamOut& fo)
    : context(ctxin), fileOut(fo) {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl)
    {
        if (!EmitIncludes && !context.getSourceManager().isInMainFile(decl->getLocation()))
            return true;

        if (decl->isInvalidDecl() || !decl->hasDefinition() || !decl->isCompleteDefinition())
            return true;

        if (!decl->getNumBases())
            return true;

        /* First ensure this inherits from struct athena::io::DNA */
        std::string baseDNA;
        if (!isDNARecord(decl, baseDNA))
            return true;

        /* Make sure there isn't Delete meta type */
        for (const clang::FieldDecl* field : decl->fields())
        {
            clang::QualType qualType = field->getType().getCanonicalType();
            const clang::Type* regType = qualType.getTypePtrOrNull();
            if (regType)
            {
                const clang::CXXRecordDecl* rDecl = regType->getAsCXXRecordDecl();
                if (rDecl)
                {
                    if (!rDecl->getName().compare("Delete"))
                    {
                        const clang::CXXRecordDecl* rParentDecl = llvm::dyn_cast_or_null<clang::CXXRecordDecl>(rDecl->getParent());
                        if (rParentDecl)
                        {
                            std::string parentCheck = rParentDecl->getTypeForDecl()->getCanonicalTypeInternal().getAsString();
                            if (!parentCheck.compare(0, sizeof(ATHENA_DNA_BASETYPE)-1, ATHENA_DNA_BASETYPE))
                                return true;
                        }
                    }
                }
            }
        }

        emitEnumerateFunc(decl, baseDNA);
        emitLookupFunc(decl, baseDNA);
        fileOut << "AT_SPECIALIZE_DNA(" << decl->getQualifiedNameAsString() << ")\n\n";

        return true;
    }
};

class ATDNAConsumer : public clang::ASTConsumer
{
    std::unique_ptr<StreamOut> fileOut;
    StreamOut& fileOutOld;
    ATDNAEmitVisitor emitVisitor;
public:
    explicit ATDNAConsumer(clang::ASTContext& context, std::unique_ptr<StreamOut>&& fo, StreamOut* foOld)
    : fileOut(std::move(fo)),
      fileOutOld(*foOld),
      emitVisitor(context, *foOld) {}

    void HandleTranslationUnit(clang::ASTContext& context) override
    {
        /* Write file head */
        fileOutOld << "/* Auto generated atdna implementation */\n"
                      "#include \"athena/DNAOp.hpp\"\n";
        for (const std::string& inputf : InputFilenames)
            fileOutOld << "#include \"" << inputf << "\"\n";
        fileOutOld << "\n";

        /* Emit file */
        emitVisitor.TraverseDecl(context.getTranslationUnitDecl());
    }
};

class ATDNAAction : public clang::ASTFrontendAction
{
    /* Used by LLVM 3.9+; client owns stream */
    std::unique_ptr<StreamOut> MakeStreamOut(std::unique_ptr<StreamOut>&& so, StreamOut*& outPtr)
    {
        outPtr = so.get();
        return std::move(so);
    }

    /* Used by previous versions of LLVM; CompilerInstance owns stream */
    std::unique_ptr<StreamOut> MakeStreamOut(StreamOut* so, StreamOut*& outPtr)
    {
        outPtr = so;
        return {};
    }

    std::unique_ptr<clang::DependencyFileGenerator> TheDependencyFileGenerator;

public:
    explicit ATDNAAction() = default;
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler,
                                                          llvm::StringRef /*filename*/) override
    {
        clang::DependencyOutputOptions DepOpts;
        DepOpts.OutputFile = DepFileOut.getValue();
        DepOpts.Targets = DepFileTargets;
        if (!DepOpts.OutputFile.empty())
            TheDependencyFileGenerator.reset(
                clang::DependencyFileGenerator::CreateAndAttachToPreprocessor(compiler.getPreprocessor(), DepOpts));

        std::unique_ptr<StreamOut> fileout;
        StreamOut* fileoutOld;
        if (OutputFilename.size())
            fileout = MakeStreamOut(compiler.createOutputFile(OutputFilename, false, true, "", "", true), fileoutOld);
        else
            fileout = MakeStreamOut(compiler.createDefaultOutputFile(false, "a", "cpp"), fileoutOld);
        AthenaError = compiler.getASTContext().getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Error, "Athena error: %0");
        return std::unique_ptr<clang::ASTConsumer>(new ATDNAConsumer(compiler.getASTContext(), std::move(fileout), fileoutOld));
    }
};

int main(int argc, const char** argv)
{
    llvm::cl::ParseCommandLineOptions(argc, argv, "Athena DNA Generator");
    if (Help)
        llvm::cl::PrintHelpMessage();

    std::vector<std::string> args = {"clang-tool",
#ifdef __linux__
                                     "--gcc-toolchain=/usr",
#endif
                                     "-fsyntax-only",
                                     "-std=c++1z",
                                     "-D__atdna__=1",
                                     "-Wno-expansion-to-defined",
                                     "-Wno-nullability-completeness",
                                     "-Werror=shadow-field",
                                     "-I" XSTR(INSTALL_PREFIX) "/lib/clang/" CLANG_VERSION_STRING "/include",
                                     "-I" XSTR(INSTALL_PREFIX) "/include/Athena"};
    for (int a=1 ; a<argc ; ++a)
        args.push_back(argv[a]);

    llvm::IntrusiveRefCntPtr<clang::FileManager> fman(new clang::FileManager(clang::FileSystemOptions()));
    ATDNAAction* action = new ATDNAAction();
    clang::tooling::ToolInvocation TI(args, action, fman.get());
    if (!TI.run())
        return 1;

    return 0;
}
