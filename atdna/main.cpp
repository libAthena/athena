#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Sema/Sema.h"
#include "clang/AST/RecordLayout.h"
#include "clang/AST/DeclCXX.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/CommandLine.h"
#include <map>

static llvm::cl::opt<bool> Help("h", llvm::cl::desc("Alias for -help"), llvm::cl::Hidden);

static llvm::cl::OptionCategory ATDNAFormatCategory("atdna options");

static llvm::cl::opt<std::string> OutputFilename("o",
                                                 llvm::cl::desc("Specify output filename"),
                                                 llvm::cl::value_desc("filename"),
                                                 llvm::cl::Prefix);
static llvm::cl::list<std::string> InputFilenames(llvm::cl::Positional,
                                                  llvm::cl::desc("<Input files>"),
                                                  llvm::cl::OneOrMore);

class ATDNAEndianVisitor : public clang::RecursiveASTVisitor<ATDNAEndianVisitor>
{
    clang::ASTContext& context;
    std::map<const clang::CXXRecordDecl*, int>& declTracker;
public:
    explicit ATDNAEndianVisitor(clang::ASTContext& ctxin,
                                std::map<const clang::CXXRecordDecl*, int>& dt)
    : context(ctxin), declTracker(dt) {}

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
                const clang::CXXRecordDecl* recordDecl = canonType.getTypePtr()->getAsCXXRecordDecl();
                if (recordDecl->getTemplateSpecializationKind())
                {
                    const clang::ClassTemplateSpecializationDecl* specDecl = (const clang::ClassTemplateSpecializationDecl*)recordDecl;
                    const clang::TemplateArgumentList& templateArgs = specDecl->getTemplateInstantiationArgs();
                    if (templateArgs.size())
                    {
                        const clang::TemplateArgument& arg = templateArgs.get(0);
                        if (arg.getKind() == clang::TemplateArgument::Integral)
                        {
                            llvm::APSInt endian = arg.getAsIntegral();
                            if (endian == 1) /* LittleEndian */
                                declTracker[recordDecl] = 1;
                            else if (endian == 2) /* BigEndian */
                                declTracker[recordDecl] = 2;
                        }
                    }
                }

                break;
            }
        }
        return true;
    }
};

class ATDNAEmitVisitor : public clang::RecursiveASTVisitor<ATDNAEmitVisitor>
{
    clang::ASTContext& context;
    llvm::raw_pwrite_stream& fileOut;
    std::map<const clang::CXXRecordDecl*, int>& declTracker;

    static std::string GetOpString(clang::BuiltinType* bType, unsigned width,
                                   std::string fieldName, bool writerPass)
    {
        if (writerPass)
        {
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
        else
        {
            if (bType->isUnsignedInteger())
            {
                if (width == 8)
                    return fieldName + " = reader.readUByte();";
                else if (width == 16)
                    return fieldName + " = reader.readUint16();";
                else if (width == 32)
                    return fieldName + " = reader.readUint32();";
                else if (width == 64)
                    return fieldName + " = reader.readUint64();";
            }
            else if (bType->isSignedInteger())
            {
                if (width == 8)
                    return fieldName + " = reader.readByte();";
                else if (width == 16)
                    return fieldName + " = reader.readInt16();";
                else if (width == 32)
                    return fieldName + " = reader.readInt32();";
                else if (width == 64)
                    return fieldName + " = reader.readInt64();";
            }
            else if (bType->isFloatingPoint())
            {
                if (width == 32)
                    return fieldName + " = reader.readFloat();";
                else if (width == 64)
                    return fieldName + " = reader.readDouble();";
            }
        }
        return std::string();
    }

public:
    explicit ATDNAEmitVisitor(clang::ASTContext& ctxin,
                              llvm::raw_pwrite_stream& fo,
                              std::map<const clang::CXXRecordDecl*, int>& dt)
    : context(ctxin), fileOut(fo), declTracker(dt) {}

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

        /* Context endian */
        int contextEndian = llvm::sys::IsLittleEndianHost ? 1 : 2;
        if (declTracker.find(decl) != declTracker.end())
            contextEndian = declTracker[decl];

        for (int p=0 ; p<2 ; ++p)
        {
            if (p)
                fileOut << decl->getQualifiedNameAsString() << "::write(Athena::IStreamWriter& writer)\n{\n";
            else
                fileOut << decl->getQualifiedNameAsString() << "::read(Athena::IStreamReader& reader)\n{\n";
            int currentEndian = 0;

            for (const clang::FieldDecl* field : decl->fields())
            {
                clang::QualType qualType = field->getType();
                clang::TypeInfo regTypeInfo = context.getTypeInfo(qualType);
                const clang::Type* regType = qualType.getTypePtrOrNull();
                if (regType->getTypeClass() == clang::Type::TemplateSpecialization)
                {
                    const clang::TemplateSpecializationType* tsType = (const clang::TemplateSpecializationType*)regType;
                    const clang::TemplateDecl* tsDecl = tsType->getTemplateName().getAsTemplateDecl();
                    const clang::TemplateParameterList* classParms = tsDecl->getTemplateParameters();
                    int endian = 0;
                    for (const clang::NamedDecl* param : *classParms)
                    {
                        if (param->getKind() == clang::Decl::NonTypeTemplateParm)
                        {
                            const clang::NonTypeTemplateParmDecl* nttParm = (clang::NonTypeTemplateParmDecl*)param;
                            const clang::Expr* defArg = nttParm->getDefaultArgument();
                            llvm::APSInt result;
                            if (defArg->isIntegerConstantExpr(result, context))
                                endian = result.getExtValue();
                        }
                    }

                    if (!tsDecl->getNameAsString().compare("Value"))
                    {

                        std::string ioOp;
                        const clang::TemplateArgument* typeArg = nullptr;
                        for (const clang::TemplateArgument& arg : *tsType)
                        {
                            if (arg.getKind() == clang::TemplateArgument::Type)
                            {
                                typeArg = &arg;
                                clang::BuiltinType* builtinType = (clang::BuiltinType*)arg.getAsType().getCanonicalType().getTypePtr();
                                if (builtinType->isBuiltinType())
                                    ioOp = GetOpString(builtinType, regTypeInfo.Width, field->getName().str(), p);
                            }
                            else if (arg.getKind() == clang::TemplateArgument::Expression)
                            {
                                llvm::APSInt value;
                                if (arg.getAsExpr()->isIntegerConstantExpr(value, context))
                                    endian = value.getExtValue();
                            }
                        }

                        if (ioOp.empty())
                        {
                            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(tsDecl->getLocation(), context.getDiagnostics().getDiagnosticIDs()->getCustomDiagID(clang::DiagnosticIDs::Fatal, "Athena error"));
                            diag.AddString("Unable to use type '" + tsDecl->getNameAsString() + "' with Athena");
                            if (typeArg)
                                diag.AddSourceRange(clang::CharSourceRange(clang::TemplateArgumentLoc(*typeArg, clang::TemplateArgumentLocInfo()).getSourceRange(), false));
                            continue;
                        }

                        if (!endian)
                            endian = contextEndian;
                        if (endian && currentEndian != endian)
                        {
                            if (endian == 1)
                                fileOut << (p ? "    writer.setEndian(Athena::LittleEndian);\n" : "    reader.setEndian(Athena::LittleEndian);\n");
                            else if (endian == 2)
                                fileOut << (p ? "    writer.setEndian(Athena::BigEndian);\n" : "    reader.setEndian(Athena::BigEndian);\n");
                            currentEndian = endian;
                        }

                        fileOut << "    " << ioOp << "\n";

                    }
                    else if (!tsDecl->getNameAsString().compare("Vector"))
                    {

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
    ATDNAEndianVisitor endianVisitor;
    ATDNAEmitVisitor emitVisitor;
    llvm::raw_pwrite_stream& fileOut;
    std::map<const clang::CXXRecordDecl*, int> declTracker;
public:
    explicit ATDNAConsumer(clang::ASTContext& context, llvm::raw_pwrite_stream& fo)
    : endianVisitor(context, declTracker), emitVisitor(context, fo, declTracker),
      fileOut(fo) {}
    void HandleTranslationUnit(clang::ASTContext& context)
    {
        /* First pass - map explicit endian specifiers */
        endianVisitor.TraverseDecl(context.getTranslationUnitDecl());

        /* Resolve endian specifiers */
        for (auto decl : declTracker)
        {
            for (const clang::DeclContext* prevDecl = decl.first->getParent();
                 prevDecl ; prevDecl = prevDecl->getParent())
            {
                if (prevDecl->isRecord() &&
                    declTracker.find((clang::CXXRecordDecl*)prevDecl) != declTracker.end())
                {
                    decl.second = declTracker[(clang::CXXRecordDecl*)prevDecl];
                    break;
                }
            }
        }

        /* Write file head */
        fileOut << "/* Auto generated atdna implementation */\n"
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
        llvm::raw_pwrite_stream* fileout;
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

    if (argc > 1)
    {
        llvm::IntrusiveRefCntPtr<clang::FileManager> fman(new clang::FileManager(clang::FileSystemOptions()));
        std::vector<std::string> args = {"clang-tool",
                                         "-fsyntax-only",
                                         "-std=c++11",
                                         "-I/run/media/jacko/Extra/llvm-build/usrmin/lib/clang/3.7.0/include",
                                         "-I/home/jacko/Athena/include"};
        for (int a=1 ; a<argc ; ++a)
            args.push_back(argv[a]);
        clang::tooling::ToolInvocation TI(args, new ATDNAAction, fman.get());
        if (TI.run())
            return 0;
    }

    return -1;
}

