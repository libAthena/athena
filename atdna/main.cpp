#include <stdio.h>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Sema/Sema.h"
#include "clang/AST/RecordLayout.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/TypeLoc.h"
#include "clang/Basic/Version.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/CommandLine.h"

#ifndef INSTALL_PREFIX
#define INSTALL_PREFIX /usr/local
#endif
#define XSTR(s) STR(s)
#define STR(s) #s

static llvm::cl::opt<bool> Help("h", llvm::cl::desc("Alias for -help"), llvm::cl::Hidden);

static llvm::cl::OptionCategory ATDNAFormatCategory("atdna options");

static llvm::cl::opt<std::string> OutputFilename("o",
                                                 llvm::cl::desc("Specify output filename"),
                                                 llvm::cl::value_desc("filename"),
                                                 llvm::cl::Prefix);

static llvm::cl::list<std::string> InputFilenames(llvm::cl::Positional,
                                                  llvm::cl::desc("<Input files>"),
                                                  llvm::cl::OneOrMore);

static llvm::cl::list<std::string> IncludeSearchPaths("I",
                                                      llvm::cl::desc("Header search path"),
                                                      llvm::cl::Prefix);

static unsigned AthenaError = 0;
#define ATHENA_DNA_BASETYPE "struct Athena::io::DNA"

class ATDNAEmitVisitor : public clang::RecursiveASTVisitor<ATDNAEmitVisitor>
{
    clang::ASTContext& context;
    llvm::raw_fd_ostream& fileOut;

    std::string GetOpString(const clang::Type* theType, unsigned width,
                            const std::string& fieldName, bool writerPass,
                            bool& isDNAType)
    {
        isDNAType = false;
        if (writerPass)
        {
            if (theType->isBuiltinType())
            {
                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isBooleanType())
                {
                    return "writer.writeBool(" + fieldName + ");";
                }
                else if (bType->isUnsignedInteger())
                {
                    if (width == 8)
                        return "writer.writeUByte(" + fieldName + ");";
                    else if (width == 16)
                        return "writer.writeUint16(" + fieldName + ");";
                    else if (width == 32)
                        return "writer.writeUint32(" + fieldName + ");";
                    else if (width == 64)
                        return "writer.writeUint64(" + fieldName + ");";
                }
                else if (bType->isSignedInteger())
                {
                    if (width == 8)
                        return "writer.writeByte(" + fieldName + ");";
                    else if (width == 16)
                        return "writer.writeInt16(" + fieldName + ");";
                    else if (width == 32)
                        return "writer.writeInt32(" + fieldName + ");";
                    else if (width == 64)
                        return "writer.writeInt64(" + fieldName + ");";
                }
                else if (bType->isFloatingPoint())
                {
                    if (width == 32)
                        return "writer.writeFloat(" + fieldName + ");";
                    else if (width == 64)
                        return "writer.writeDouble(" + fieldName + ");";
                }
            }
            else if (theType->isRecordType())
            {
                const clang::CXXRecordDecl* rDecl = theType->getAsCXXRecordDecl();
                for (const clang::FieldDecl* field : rDecl->fields())
                {
                    if (!field->getNameAsString().compare("clangVec"))
                    {
                        const clang::VectorType* vType = (clang::VectorType*)field->getType().getTypePtr();
                        if (vType->isVectorType())
                        {
                            const clang::BuiltinType* eType = (clang::BuiltinType*)vType->getElementType().getTypePtr();
                            if (!eType->isBuiltinType() || !eType->isFloatingPoint() ||
                                context.getTypeInfo(eType).Width != 32)
                                continue;
                            if (vType->getNumElements() == 3)
                                return "writer.writeVec3f(" + fieldName + ");";
                            else if (vType->getNumElements() == 4)
                                return "writer.writeVec4f(" + fieldName + ");";
                        }
                    }
                }
                for (const clang::CXXBaseSpecifier& base : rDecl->bases())
                    if (!base.getType().getCanonicalType().getAsString().compare(0, sizeof(ATHENA_DNA_BASETYPE)-1, ATHENA_DNA_BASETYPE))
                    {
                        isDNAType = true;
                        return "write(writer);";
                    }
            }
        }
        else
        {
            if (theType->isBuiltinType())
            {
                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isBooleanType())
                {
                    return "reader.readBool()";
                }
                else if (bType->isUnsignedInteger())
                {
                    if (width == 8)
                        return "reader.readUByte()";
                    else if (width == 16)
                        return "reader.readUint16()";
                    else if (width == 32)
                        return "reader.readUint32()";
                    else if (width == 64)
                        return "reader.readUint64()";
                }
                else if (bType->isSignedInteger())
                {
                    if (width == 8)
                        return "reader.readByte()";
                    else if (width == 16)
                        return "reader.readInt16()";
                    else if (width == 32)
                        return "reader.readInt32()";
                    else if (width == 64)
                        return "reader.readInt64()";
                }
                else if (bType->isFloatingPoint())
                {
                    if (width == 32)
                        return "reader.readFloat()";
                    else if (width == 64)
                        return "reader.readDouble()";
                }
            }
            else if (theType->isRecordType())
            {
                const clang::CXXRecordDecl* rDecl = theType->getAsCXXRecordDecl();
                for (const clang::FieldDecl* field : rDecl->fields())
                {
                    if (!field->getNameAsString().compare("clangVec"))
                    {
                        const clang::VectorType* vType = (clang::VectorType*)field->getType().getTypePtr();
                        if (vType->isVectorType())
                        {
                            const clang::BuiltinType* eType = (clang::BuiltinType*)vType->getElementType().getTypePtr();
                            if (!eType->isBuiltinType() || !eType->isFloatingPoint() ||
                                context.getTypeInfo(eType).Width != 32)
                                continue;
                            if (vType->getNumElements() == 3)
                                return "reader.readVec3f()";
                            else if (vType->getNumElements() == 4)
                                return "reader.readVec4f()";
                        }
                    }
                }
                for (const clang::CXXBaseSpecifier& base : rDecl->bases())
                    if (!base.getType().getCanonicalType().getAsString().compare(0, sizeof(ATHENA_DNA_BASETYPE)-1, ATHENA_DNA_BASETYPE))
                    {
                        isDNAType = true;
                        return "read(reader);";
                    }
            }
        }
        return std::string();
    }

public:
    explicit ATDNAEmitVisitor(clang::ASTContext& ctxin,
                              llvm::raw_fd_ostream& fo)
    : context(ctxin), fileOut(fo) {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl)
    {
        if (decl->isInvalidDecl() || !decl->hasDefinition())
            return true;

        if (!decl->getNumBases())
            return true;

        /* First ensure this inherits from struct Athena::io::DNA */
        bool foundDNA = false;
        for (const clang::CXXBaseSpecifier& base : decl->bases())
        {
            clang::QualType canonType = base.getType().getCanonicalType();
            if (!canonType.getAsString().compare(0, sizeof(ATHENA_DNA_BASETYPE)-1, ATHENA_DNA_BASETYPE))
            {
                foundDNA = true;
                break;
            }
        }
        if (!foundDNA)
            return true;

        /* Two passes - read then write */
        for (int p=0 ; p<2 ; ++p)
        {
            if (p)
                fileOut << "void " << decl->getQualifiedNameAsString() << "::write(Athena::io::IStreamWriter& writer) const\n{\n";
            else
                fileOut << "void " << decl->getQualifiedNameAsString() << "::read(Athena::io::IStreamReader& reader)\n{\n";
            int currentEndian = -1;

            for (const clang::FieldDecl* field : decl->fields())
            {
                clang::QualType qualType = field->getType();
                clang::TypeInfo regTypeInfo = context.getTypeInfo(qualType);
                const clang::Type* regType = qualType.getTypePtrOrNull();
                if (regType->getTypeClass() == clang::Type::Elaborated)
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
                        fieldName = field->getNameAsString() + subscript;
                    }
                    else
                        fieldName = field->getNameAsString();

                    if (regType->getTypeClass() == clang::Type::TemplateSpecialization)
                    {
                        const clang::TemplateSpecializationType* tsType = (const clang::TemplateSpecializationType*)regType;
                        const clang::TemplateDecl* tsDecl = tsType->getTemplateName().getAsTemplateDecl();
                        const clang::TemplateParameterList* classParms = tsDecl->getTemplateParameters();

                        if (!tsDecl->getNameAsString().compare("Value"))
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
                                        if (!p)
                                        {
                                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(defArg->getLocStart(), AthenaError);
                                            diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                            diag.AddSourceRange(clang::CharSourceRange(defArg->getSourceRange(), true));
                                        }
                                        continue;
                                    }
                                }
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
                                    const clang::Type* type = arg.getAsType().getCanonicalType().getTypePtr();
                                    ioOp = GetOpString(type, regTypeInfo.Width, fieldName, p, isDNAType);
                                }
                                else if (arg.getKind() == clang::TemplateArgument::Expression)
                                {
                                    const clang::Expr* expr = arg.getAsExpr();
                                    endianExpr = expr;
                                    if (expr->isIntegerConstantExpr(endian, context))
                                    {
                                        if (!p)
                                        {
                                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                            diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                            diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                        }
                                        continue;
                                    }
                                }
                            }

                            int endianVal = endian.getSExtValue();
                            if (endianVal != 0 && endianVal != 1)
                            {
                                if (!p)
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
                                }
                                continue;
                            }

                            if (ioOp.empty())
                            {
                                if (!p)
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                    diag.AddString("Unable to use type '" + tsDecl->getNameAsString() + "' with Athena");
                                    diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                                }
                                continue;
                            }

                            if (currentEndian != endianVal)
                            {
                                if (endianVal == 0)
                                    fileOut << (p ? "    writer.setEndian(Athena::LittleEndian);\n" : "    reader.setEndian(Athena::LittleEndian);\n");
                                else if (endianVal == 1)
                                    fileOut << (p ? "    writer.setEndian(Athena::BigEndian);\n" : "    reader.setEndian(Athena::BigEndian);\n");
                                currentEndian = endianVal;
                            }

                            fileOut << "    /* " << fieldName << " */\n";
                            if (!p)
                                fileOut << "    " << fieldName << " = " << ioOp << ";\n";
                            else
                                fileOut << "    " << ioOp << "\n";
                        }
                        else if (!tsDecl->getNameAsString().compare("Vector"))
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
                                        if (!p)
                                        {
                                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(defArg->getLocStart(), AthenaError);
                                            diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                            diag.AddSourceRange(clang::CharSourceRange(defArg->getSourceRange(), true));
                                        }
                                        continue;
                                    }
                                }
                            }

                            clang::QualType templateType;
                            std::string ioOp;
                            bool isDNAType = false;
                            std::string sizeExpr;
                            const clang::TemplateArgument* typeArg = nullptr;
                            const clang::TemplateArgument* sizeArg = nullptr;
                            size_t idx = 0;
                            bool bad = false;
                            for (const clang::TemplateArgument& arg : *tsType)
                            {
                                if (arg.getKind() == clang::TemplateArgument::Type)
                                {
                                    typeArg = &arg;
                                    templateType = arg.getAsType().getCanonicalType();
                                    clang::TypeInfo typeInfo = context.getTypeInfo(templateType);
                                    static const std::string elemStr = "elem";
                                    ioOp = GetOpString(templateType.getTypePtr(), typeInfo.Width, elemStr, p, isDNAType);
                                }
                                else if (arg.getKind() == clang::TemplateArgument::Expression)
                                {
                                    const clang::Expr* expr = arg.getAsExpr();
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
                                            if (!p)
                                            {
                                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                                diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                            }
                                            bad = true;
                                            break;
                                        }
                                    }
                                }
                                ++idx;
                            }
                            if (bad)
                                continue;

                            int endianVal = endian.getSExtValue();
                            if (endianVal != 0 && endianVal != 1)
                            {
                                if (!p)
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
                                }
                                continue;
                            }

                            if (ioOp.empty())
                            {
                                if (!p)
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                    diag.AddString("Unable to use type '" + templateType.getAsString() + "' with Athena");
                                    diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                                }
                                continue;
                            }

                            if (sizeExpr.empty())
                            {
                                if (!p)
                                {
                                    clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocStart(), AthenaError);
                                    diag.AddString("Unable to use count variable with Athena");
                                    diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
                                }
                                continue;
                            }

                            if (currentEndian != endianVal)
                            {
                                if (endianVal == 0)
                                    fileOut << (p ? "    writer.setEndian(Athena::LittleEndian);\n" : "    reader.setEndian(Athena::LittleEndian);\n");
                                else if (endianVal == 1)
                                    fileOut << (p ? "    writer.setEndian(Athena::BigEndian);\n" : "    reader.setEndian(Athena::BigEndian);\n");
                                currentEndian = endianVal;
                            }

                            fileOut << "    /* " << fieldName << " */\n";
                            if (!p)
                            {
                                fileOut << "    " << fieldName << ".clear();\n";
                                fileOut << "    " << fieldName << ".reserve(" << sizeExpr << ");\n";
                                if (isDNAType)
                                    fileOut << "    for (int i=0 ; i<(" << sizeExpr << ") ; ++i)\n"
                                               "    {\n"
                                               "        " << fieldName << ".emplace_back();\n"
                                               "        " << fieldName << ".back()." << ioOp << "\n"
                                               "    }\n";
                                else
                                    fileOut << "    for (int i=0 ; i<(" << sizeExpr << ") ; ++i)\n"
                                               "        " << fieldName << ".push_back(" << ioOp << ");\n";
                            }
                            else
                            {
                                fileOut << "    for (auto elem : " << fieldName << ")\n";
                                if (isDNAType)
                                    fileOut << "        elem." << ioOp << "\n";
                                else
                                    fileOut << "        " << ioOp << "\n";
                            }

                        }
                        else if (!tsDecl->getNameAsString().compare("Buffer"))
                        {
                            const clang::Expr* sizeExpr = nullptr;
                            std::string sizeExprStr;
                            for (const clang::TemplateArgument& arg : *tsType)
                            {
                                if (arg.getKind() == clang::TemplateArgument::Expression)
                                {
                                    const clang::UnaryExprOrTypeTraitExpr* uExpr = (clang::UnaryExprOrTypeTraitExpr*)arg.getAsExpr();
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
                                if (!p)
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
                                }
                                continue;
                            }

                            fileOut << "    /* " << fieldName << " */\n";
                            if (!p)
                            {
                                fileOut << "    " << fieldName << ".reset(new atUint8[" << sizeExprStr << "]);\n";
                                fileOut << "    reader.readUBytesToBuf(" << fieldName << ".get(), " << sizeExprStr << ");\n";
                            }
                            else
                            {
                                fileOut << "    writer.writeUBytes(" << fieldName << ".get(), " << sizeExprStr << ");\n";
                            }
                        }
                        else if (!tsDecl->getNameAsString().compare("String"))
                        {
                            const clang::Expr* sizeExpr = nullptr;
                            std::string sizeExprStr;
                            for (const clang::TemplateArgument& arg : *tsType)
                            {
                                if (arg.getKind() == clang::TemplateArgument::Expression)
                                {
                                    const clang::Expr* expr = arg.getAsExpr();
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

                            fileOut << "    /* " << fieldName << " */\n";
                            if (!p)
                                fileOut << "    " << fieldName << " = reader.readString(" << sizeExprStr << ");\n";
                            else
                                fileOut << "    writer.writeString(" << fieldName << ");\n";
                        }
                        else if (!tsDecl->getNameAsString().compare("WString"))
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
                                        if (!p)
                                        {
                                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(defArg->getLocStart(), AthenaError);
                                            diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                            diag.AddSourceRange(clang::CharSourceRange(defArg->getSourceRange(), true));
                                        }
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
                                    const clang::Expr* expr = arg.getAsExpr();
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
                                            if (!p)
                                            {
                                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                                diag.AddString("Endian value must be 'BigEndian' or 'LittleEndian'");
                                                diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                            }
                                            bad = true;
                                            break;
                                        }
                                    }
                                }
                                ++idx;
                            }
                            if (bad)
                                continue;

                            int endianVal = endian.getSExtValue();
                            if (endianVal != 0 && endianVal != 1)
                            {
                                if (!p)
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
                                }
                                continue;
                            }

                            if (currentEndian != endianVal)
                            {
                                if (endianVal == 0)
                                    fileOut << (p ? "    writer.setEndian(Athena::LittleEndian);\n" : "    reader.setEndian(Athena::LittleEndian);\n");
                                else if (endianVal == 1)
                                    fileOut << (p ? "    writer.setEndian(Athena::BigEndian);\n" : "    reader.setEndian(Athena::BigEndian);\n");
                                currentEndian = endianVal;
                            }

                            fileOut << "    /* " << fieldName << " */\n";
                            if (!p)
                                fileOut << "    " << fieldName << " = reader.readWString(" << sizeExprStr << ");\n";
                            else
                                fileOut << "    writer.writeWString(" << fieldName << ");\n";
                        }
                        else if (!tsDecl->getNameAsString().compare("UTF8"))
                        {
                            const clang::Expr* sizeExpr = nullptr;
                            std::string sizeExprStr;
                            for (const clang::TemplateArgument& arg : *tsType)
                            {
                                if (arg.getKind() == clang::TemplateArgument::Expression)
                                {
                                    const clang::Expr* expr = arg.getAsExpr();
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

                            fileOut << "    /* " << fieldName << " */\n";
                            if (!p)
                                fileOut << "    " << fieldName << " = reader.readUnicode(" << sizeExprStr << ");\n";
                            else
                                fileOut << "    writer.writeUnicode(" << fieldName << ");\n";
                        }
                        else if (!tsDecl->getNameAsString().compare("Seek"))
                        {
                            size_t idx = 0;
                            llvm::APSInt offset(64, 0);
                            llvm::APSInt direction(64, 0);
                            const clang::Expr* directionExpr = nullptr;
                            bool bad = false;
                            for (const clang::TemplateArgument& arg : *tsType)
                            {
                                if (arg.getKind() == clang::TemplateArgument::Expression)
                                {
                                    const clang::Expr* expr = arg.getAsExpr();
                                    if (!idx)
                                    {
                                        if (!expr->isIntegerConstantExpr(offset, context))
                                        {
                                            if (!p)
                                            {
                                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                                diag.AddString("Unable to use non-constant offset expression in Athena");
                                                diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                            }
                                            bad = true;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        directionExpr = expr;
                                        if (!expr->isIntegerConstantExpr(direction, context))
                                        {
                                            if (!p)
                                            {
                                                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                                diag.AddString("Unable to use non-constant direction expression in Athena");
                                                diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                            }
                                            bad = true;
                                            break;
                                        }
                                    }
                                }
                                ++idx;
                            }
                            if (bad)
                                continue;

                            int64_t offsetVal = offset.getSExtValue();
                            int64_t directionVal = direction.getSExtValue();
                            if (directionVal < 0 || directionVal > 2)
                            {
                                if (!p)
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
                                }
                                continue;
                            }

                            fileOut << "    /* " << fieldName << " */\n";
                            if (directionVal == 0)
                            {
                                if (!p)
                                    fileOut << "    reader.seek(" << offsetVal << ", Athena::Begin);\n";
                                else
                                    fileOut << "    writer.seek(" << offsetVal << ", Athena::Begin);\n";
                            }
                            else if (directionVal == 1)
                            {
                                if (!p)
                                    fileOut << "    reader.seek(" << offsetVal << ", Athena::Current);\n";
                                else
                                    fileOut << "    writer.seek(" << offsetVal << ", Athena::Current);\n";
                            }
                            else if (directionVal == 2)
                            {
                                if (!p)
                                    fileOut << "    reader.seek(" << offsetVal << ", Athena::End);\n";
                                else
                                    fileOut << "    writer.seek(" << offsetVal << ", Athena::End);\n";
                            }

                        }
                        else if (!tsDecl->getNameAsString().compare("Align"))
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
                                        if (!p)
                                        {
                                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getLocStart(), AthenaError);
                                            diag.AddString("Unable to use non-constant align expression in Athena");
                                            diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                                        }
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
                                fileOut << "    /* " << fieldName << " */\n";
                                if (alignVal == 32)
                                {
                                    if (!p)
                                        fileOut << "    reader.seekAlign32();\n";
                                    else
                                        fileOut << "    writer.seekAlign32();\n";
                                }
                                else if (align.isPowerOf2())
                                {
                                    if (!p)
                                        fileOut << "    reader.seek((reader.position() + " << alignVal-1 << ") & ~" << alignVal-1 << ", Athena::Begin);\n";
                                    else
                                        fileOut << "    writer.seek((writer.position() + " << alignVal-1 << ") & ~" << alignVal-1 << ", Athena::Begin);\n";
                                }
                                else
                                {
                                    if (!p)
                                        fileOut << "    reader.seek((reader.position() + " << alignVal-1 << ") / " << alignVal << " * " << alignVal << ", Athena::Begin);\n";
                                    else
                                        fileOut << "    writer.seek((writer.position() + " << alignVal-1 << ") / " << alignVal << " * " << alignVal << ", Athena::Begin);\n";
                                }
                            }
                        }

                    }

                    else if (regType->getTypeClass() == clang::Type::Record)
                    {
                        const clang::CXXRecordDecl* cxxRDecl = regType->getAsCXXRecordDecl();
                        if (cxxRDecl)
                        {
                            for (const clang::CXXBaseSpecifier& base : cxxRDecl->bases())
                            {
                                clang::QualType canonType = base.getType().getCanonicalType();
                                if (!canonType.getAsString().compare(0, sizeof(ATHENA_DNA_BASETYPE)-1, ATHENA_DNA_BASETYPE))
                                {
                                    fileOut << "    /* " << fieldName << " */\n";
                                    fileOut << "    " << fieldName << (p ? ".write(writer);\n" : ".read(reader);\n");
                                    currentEndian = -1;
                                    break;
                                }
                            }
                        }
                    }

                }

            }

            fileOut << "}\n\n";

        }

        return true;
    }
};

class ATDNAConsumer : public clang::ASTConsumer
{
    ATDNAEmitVisitor emitVisitor;
    llvm::raw_fd_ostream& fileOut;
public:
    explicit ATDNAConsumer(clang::ASTContext& context, llvm::raw_fd_ostream& fo)
    : emitVisitor(context, fo),
      fileOut(fo) {}
    void HandleTranslationUnit(clang::ASTContext& context)
    {
        /* Write file head */
        fileOut << "/* Auto generated atdna implementation */\n"
                   "#include <Athena/Global.hpp>\n"
                   "#include <Athena/IStreamReader.hpp>\n"
                   "#include <Athena/IStreamWriter.hpp>\n\n";
        for (const std::string& inputf : InputFilenames)
            fileOut << "#include \"" << inputf << "\"\n";
        fileOut << "\n";

        /* Emit file */
        emitVisitor.TraverseDecl(context.getTranslationUnitDecl());
    }
};

class ATDNAAction : public clang::ASTFrontendAction
{
public:
    explicit ATDNAAction() {}
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler,
                                                          llvm::StringRef /*filename*/)
    {
        llvm::raw_fd_ostream* fileout;
        if (OutputFilename.size())
            fileout = compiler.createOutputFile(OutputFilename, false, true, "", "", true);
        else
            fileout = compiler.createDefaultOutputFile(false, "a", "cpp");
        AthenaError = compiler.getASTContext().getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Error, "Athena error: %0");
        return std::unique_ptr<clang::ASTConsumer>(new ATDNAConsumer(compiler.getASTContext(), *fileout));
    }
};


int main(int argc, const char** argv)
{
    llvm::cl::ParseCommandLineOptions(argc, argv, "Athena DNA Generator");
    if (Help)
        llvm::cl::PrintHelpMessage();

    std::vector<std::string> args = {"clang-tool",
                                     "-fsyntax-only",
                                     "-std=c++11"};
    args.push_back("-I" + std::string(XSTR(INSTALL_PREFIX)) + "/lib/clang/" + CLANG_VERSION_STRING + "/include");
    args.push_back("-I" + std::string(XSTR(INSTALL_PREFIX)) + "/include/Athena");
    for (int a=1 ; a<argc ; ++a)
        args.push_back(argv[a]);

    llvm::IntrusiveRefCntPtr<clang::FileManager> fman(new clang::FileManager(clang::FileSystemOptions()));
    clang::tooling::ToolInvocation TI(args, new ATDNAAction, fman.get());
    if (TI.run())
        return 0;

    return -1;
}

