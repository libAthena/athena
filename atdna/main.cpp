#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Sema/Sema.h"
#include "clang/AST/RecordLayout.h"
#include "clang/AST/DeclCXX.h"
#include "clang/Basic/Version.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/CommandLine.h"

static llvm::cl::opt<bool> Help("h", llvm::cl::desc("Alias for -help"), llvm::cl::Hidden);

static llvm::cl::OptionCategory ATDNAFormatCategory("atdna options");

static llvm::cl::opt<std::string> OutputFilename("o",
                                                 llvm::cl::desc("Specify output filename"),
                                                 llvm::cl::value_desc("filename"),
                                                 llvm::cl::Prefix);

static llvm::cl::list<std::string> InputFilenames(llvm::cl::Positional,
                                                  llvm::cl::desc("<Input files>"),
                                                  llvm::cl::OneOrMore);

class ATDNAEmitVisitor : public clang::RecursiveASTVisitor<ATDNAEmitVisitor>
{
    clang::ASTContext& context;
    llvm::raw_fd_ostream& fileOut;

    std::string GetOpString(const clang::Type* theType, unsigned width,
                            std::string fieldName, bool writerPass)
    {
        if (writerPass)
        {
            if (theType->isBuiltinType())
            {
                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isUnsignedInteger())
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
                const clang::RecordDecl* rDecl = ((clang::RecordType*)theType)->getDecl();
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
            }
        }
        else
        {
            if (theType->isBuiltinType())
            {
                const clang::BuiltinType* bType = (clang::BuiltinType*)theType;
                if (bType->isUnsignedInteger())
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
                const clang::RecordDecl* rDecl = ((clang::RecordType*)theType)->getDecl();
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
            if (!canonType.getAsString().compare(0, 22, "struct Athena::io::DNA"))
            {
                foundDNA = true;
                break;
            }
        }
        if (!foundDNA)
            return true;

        for (int p=0 ; p<2 ; ++p)
        {
            if (p)
                fileOut << "void " << decl->getQualifiedNameAsString() << "::write(Athena::IStreamWriter& writer) const\n{\n";
            else
                fileOut << "void " << decl->getQualifiedNameAsString() << "::read(Athena::IStreamReader& reader)\n{\n";
            int currentEndian = -1;

            for (const clang::FieldDecl* field : decl->fields())
            {
                clang::QualType qualType = field->getType();
                clang::TypeInfo regTypeInfo = context.getTypeInfo(qualType);
                const clang::Type* regType = qualType.getTypePtrOrNull();
                if (regType->getTypeClass() == clang::Type::Elaborated)
                    regType = regType->getUnqualifiedDesugaredType();

                if (regType->getTypeClass() == clang::Type::TemplateSpecialization)
                {
                    const clang::TemplateSpecializationType* tsType = (const clang::TemplateSpecializationType*)regType;
                    const clang::TemplateDecl* tsDecl = tsType->getTemplateName().getAsTemplateDecl();
                    const clang::TemplateParameterList* classParms = tsDecl->getTemplateParameters();

                    if (!tsDecl->getNameAsString().compare("Value"))
                    {
                        int endian = -1;
                        if (classParms->size() >= 2)
                        {
                            const clang::NamedDecl* endianParm = classParms->getParam(1);
                            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm)
                            {
                                const clang::NonTypeTemplateParmDecl* nttParm = (clang::NonTypeTemplateParmDecl*)endianParm;
                                const clang::Expr* defArg = nttParm->getDefaultArgument();
                                llvm::APSInt result;
                                if (defArg->isIntegerConstantExpr(result, context))
                                    endian = result.getSExtValue();
                            }
                        }

                        std::string ioOp;
                        const clang::TemplateArgument* typeArg = nullptr;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Type)
                            {
                                typeArg = &arg;
                                const clang::Type* type = arg.getAsType().getCanonicalType().getTypePtr();
                                ioOp = GetOpString(type, regTypeInfo.Width, field->getName().str(), p);
                            }
                            else if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                llvm::APSInt value;
                                if (arg.getAsExpr()->isIntegerConstantExpr(value, context))
                                    endian = value.getSExtValue();
                            }
                        }

                        if (ioOp.empty())
                        {
                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(tsDecl->getLocation(), context.getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Fatal, "Athena error"));
                            diag.AddString("Unable to use type '" + tsDecl->getNameAsString() + "' with Athena");
                            if (typeArg)
                                diag.AddSourceRange(clang::CharSourceRange(clang::TemplateArgumentLoc(*typeArg, clang::TemplateArgumentLocInfo()).getSourceRange(), false));
                            continue;
                        }

                        if (currentEndian != endian)
                        {
                            if (endian == 0)
                                fileOut << (p ? "    writer.setEndian(Athena::LittleEndian);\n" : "    reader.setEndian(Athena::LittleEndian);\n");
                            else if (endian == 1)
                                fileOut << (p ? "    writer.setEndian(Athena::BigEndian);\n" : "    reader.setEndian(Athena::BigEndian);\n");
                            currentEndian = endian;
                        }

                        if (!p)
                            fileOut << "    " << field->getName() << " = " << ioOp << ";\n";
                        else
                            fileOut << "    " << ioOp << "\n";
                    }
                    else if (!tsDecl->getNameAsString().compare("Vector"))
                    {
                        int endian = -1;
                        if (classParms->size() >= 3)
                        {
                            const clang::NamedDecl* endianParm = classParms->getParam(2);
                            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm)
                            {
                                const clang::NonTypeTemplateParmDecl* nttParm = (clang::NonTypeTemplateParmDecl*)endianParm;
                                const clang::Expr* defArg = nttParm->getDefaultArgument();
                                llvm::APSInt result;
                                if (defArg->isIntegerConstantExpr(result, context))
                                    endian = result.getSExtValue();
                            }
                        }

                        std::string ioOp;
                        std::string sizeVar;
                        const clang::TemplateArgument* typeArg = nullptr;
                        const clang::TemplateArgument* sizeArg = nullptr;
                        size_t idx = 0;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Type)
                            {
                                typeArg = &arg;
                                const clang::Type* type = arg.getAsType().getCanonicalType().getTypePtr();
                                clang::TypeInfo typeInfo = context.getTypeInfo(arg.getAsType().getCanonicalType());
                                ioOp = GetOpString(type, typeInfo.Width, "*it", p);
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
                                        if (argExpr->getStmtClass() == clang::Stmt::DeclRefExprClass)
                                        {
                                            const clang::DeclRefExpr* refExpr = (clang::DeclRefExpr*)argExpr;
                                            sizeVar = refExpr->getFoundDecl()->getNameAsString();
                                        }
                                    }
                                }
                                else if (idx == 2)
                                {
                                    llvm::APSInt value;
                                    if (expr->isIntegerConstantExpr(value, context))
                                        endian = value.getSExtValue();
                                }
                            }
                            ++idx;
                        }

                        if (ioOp.empty())
                        {
                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(tsDecl->getLocation(), context.getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Fatal, "Athena error"));
                            diag.AddString("Unable to use type '" + tsDecl->getNameAsString() + "' with Athena");
                            if (typeArg)
                                diag.AddSourceRange(clang::CharSourceRange(clang::TemplateArgumentLoc(*typeArg, clang::TemplateArgumentLocInfo()).getSourceRange(), false));
                            continue;
                        }

                        if (sizeVar.empty())
                        {
                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(tsDecl->getLocation(), context.getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Fatal, "Athena error"));
                            diag.AddString("Unable to use type '" + tsDecl->getNameAsString() + "' with Athena");
                            if (sizeArg)
                                diag.AddSourceRange(clang::CharSourceRange(clang::TemplateArgumentLoc(*sizeArg, clang::TemplateArgumentLocInfo()).getSourceRange(), false));
                            continue;
                        }

                        if (currentEndian != endian)
                        {
                            if (endian == 0)
                                fileOut << (p ? "    writer.setEndian(Athena::LittleEndian);\n" : "    reader.setEndian(Athena::LittleEndian);\n");
                            else if (endian == 1)
                                fileOut << (p ? "    writer.setEndian(Athena::BigEndian);\n" : "    reader.setEndian(Athena::BigEndian);\n");
                            currentEndian = endian;
                        }

                        if (!p)
                        {
                            fileOut << "    " << field->getName() << ".clear();\n";
                            fileOut << "    " << field->getName() << ".reserve(" << sizeVar << ");\n";
                            fileOut << "    for (int i=0 ; i<" << sizeVar << " ; ++i)\n        " << field->getName() << ".push_back(" << ioOp << ");\n";
                        }
                        else
                            fileOut << "    for (int i=0, auto it=" << field->getName() << ".begin() ; i<" << sizeVar << " && it!=" << field->getName() << ".end() ; ++i, ++it)\n        " << ioOp << "\n";

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
                            if (!canonType.getAsString().compare(0, 22, "struct Athena::io::DNA"))
                            {
                                fileOut << "    " << field->getNameAsString() << (p ? ".write(writer);\n" : ".read(reader);\n");
                                currentEndian = -1;
                                break;
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

        /* Second pass - emit file */
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

    llvm::IntrusiveRefCntPtr<clang::FileManager> fman(new clang::FileManager(clang::FileSystemOptions()));
    const clang::FileEntry* selfEntry = fman->getFile(argv[0]);
    if (selfEntry && selfEntry->isValid())
    {
        std::string base(selfEntry->getDir()->getName());
        args.push_back("-I" + base + "/clang/" + CLANG_VERSION_STRING + "/include");
        args.push_back("-I" + base);
    }

    for (int a=1 ; a<argc ; ++a)
        args.push_back(argv[a]);

    clang::tooling::ToolInvocation TI(args, new ATDNAAction, fman.get());
    if (TI.run())
        return 0;

    return -1;
}

