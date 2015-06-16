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

static llvm::cl::opt<bool> Help("h", llvm::cl::desc("Alias for -help"), llvm::cl::Hidden);

static llvm::cl::OptionCategory ATDNAFormatCategory("atdna options");

static llvm::cl::opt<std::string> OutputFilename("o",
                                                 llvm::cl::desc("Specify output filename"),
                                                 llvm::cl::value_desc("filename"),
                                                 llvm::cl::Prefix);
static llvm::cl::list<std::string> InputFilenames(llvm::cl::Positional,
                                                  llvm::cl::desc("<Input files>"),
                                                  llvm::cl::OneOrMore);

class ATDNAVisitor : public clang::RecursiveASTVisitor<ATDNAVisitor>
{
    clang::ASTContext& context;
    llvm::raw_pwrite_stream& fileOut;
public:
    explicit ATDNAVisitor(clang::ASTContext& ctxin, llvm::raw_pwrite_stream& fo)
    : context(ctxin), fileOut(fo) {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl)
    {
        if (decl->isInvalidDecl() || !decl->hasDefinition())
            return true;

        if (!decl->getNumBases())
            return true;

        bool foundDNA = false;
        for (const clang::CXXBaseSpecifier& base : decl->bases())
        {
            clang::QualType canonType = base.getType().getCanonicalType();
            //llvm::outs() << "BASE " << canonType.getAsString() << "\n";
            if (!canonType.getAsString().compare(0, 22, "struct Athena::io::DNA"))
            {
                llvm::outs() << "BASE " << canonType.getAsString() << "\n";
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
                            llvm::outs() << arg.getIntegralType().getAsString() << " INT " << arg.getAsIntegral() << "\n";
                        }
                    }
                }

                break;
            }
        }
        if (!foundDNA)
            return true;

        fileOut << "one" << "::" << "two";

        llvm::outs() << "DECL name " << decl->getQualifiedNameAsString() << "\n";
        llvm::outs() << "DECL kind " << decl->getKindName() << "\n";
        const clang::ASTRecordLayout& layout = context.getASTRecordLayout(decl);
        for (const clang::FieldDecl* field : decl->fields())
        {
            llvm::outs() << "    Field " << field->getName();
            clang::QualType qualType = field->getType();
            const clang::Type* regType = qualType.getTypePtrOrNull();
            if (regType->getTypeClass() == clang::Type::Typedef)
            {
                const clang::TypedefType* tdType = (const clang::TypedefType*)regType;
                llvm::outs() << "    Typedef " << tdType->getDecl()->getNameAsString();
            }
            else if (regType->getTypeClass() == clang::Type::TemplateSpecialization)
            {
                const clang::TemplateSpecializationType* tsType = (const clang::TemplateSpecializationType*)regType;
                const clang::TemplateDecl* tsDecl = tsType->getTemplateName().getAsTemplateDecl();
                const clang::TemplateParameterList* tsList = tsDecl->getTemplateParameters();
                llvm::outs() << "    Alias " << tsDecl->getNameAsString();
                llvm::outs() << " " << tsType->getNumArgs() << " " << tsList->size() << "\n";
                for (const clang::NamedDecl* param : *tsList)
                {
                    llvm::outs() << "    " << param->getName() << " " << param->getDeclKindName();
                    if (param->getKind() == clang::Decl::NonTypeTemplateParm)
                    {
                        const clang::NonTypeTemplateParmDecl* nttParm = (clang::NonTypeTemplateParmDecl*)param;
                        const clang::Expr* defArg = nttParm->getDefaultArgument();
                        llvm::APSInt result;
                        if (defArg->isIntegerConstantExpr(result, context))
                        {
                            llvm::outs() << " " << result;
                        }
                    }
                    llvm::outs() << "\n";
                }
                /*
                const clang::TypeAliasTemplateDecl* tsOrig = llvm::dyn_cast_or_null<clang::TypeAliasTemplateDecl>(tsType->getTemplateName().getAsTemplateDecl());
                if (tsOrig)
                {
                    const clang::TemplateParameterList* params = tsOrig->getTemplateParameters();
                    llvm::outs() << " PARAMS:\n";
                    for (const clang::NamedDecl* param : *params)
                    {
                        llvm::outs() << "    " << param->getName() << "\n";
                    }
                }
                */
                for (const clang::TemplateArgument& arg : *tsType)
                {
                    if (arg.getKind() == clang::TemplateArgument::Expression)
                    {
                        llvm::APSInt value;
                        if (arg.getAsExpr()->isIntegerConstantExpr(value, context))
                        {
                            llvm::outs() << " Expr " << value;
                        }
                    }
                    else if (arg.getKind() == clang::TemplateArgument::Type)
                    {
                        llvm::outs() << " Type " << arg.getAsType().getAsString();
                    }
                }
            }
            else if (regType->getTypeClass() == clang::Type::Builtin)
            {
                const clang::BuiltinType* bType = (const clang::BuiltinType*)regType;
                llvm::outs() << "    Builtin " << bType->getName(clang::PrintingPolicy(clang::LangOptions()));
            }
            clang::TypeInfo regTypeInfo = context.getTypeInfo(qualType);
            llvm::outs() << "    Width " << regTypeInfo.Width;
            llvm::outs() << "    Off " << layout.getFieldOffset(field->getFieldIndex()) << "\n";
        }
        /*
        if (decl->getQualifiedNameAsString() == "n::m::C")
        {
            clang::FullSourceLoc fullLoc = context.getFullLoc(decl->getLocStart());
            if (fullLoc.isValid())
                llvm::outs() << "Found declaration at "
                             << fullLoc.getSpellingLineNumber() << ":"
                             << fullLoc.getSpellingColumnNumber() << "\n";
        }
        */
        return true;
    }
};

class ATDNAConsumer : public clang::ASTConsumer
{
    ATDNAVisitor visitor;
public:
    explicit ATDNAConsumer(clang::ASTContext& context, llvm::raw_pwrite_stream& fo)
    : visitor(context, fo) {}
    void HandleTranslationUnit(clang::ASTContext& context)
    {visitor.TraverseDecl(context.getTranslationUnitDecl());}
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

