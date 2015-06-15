#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Sema/Sema.h"
#include "clang/AST/RecordLayout.h"
#include "clang/AST/DeclCXX.h"

class FindNamedClassVisitor : public clang::RecursiveASTVisitor<FindNamedClassVisitor>
{
    clang::ASTContext& context;
public:
    explicit FindNamedClassVisitor(clang::ASTContext& ctxin)
    : context(ctxin) {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl)
    {
        if (decl->isInvalidDecl() || !decl->hasDefinition())
            return true;

        if (!decl->getNumBases())
            return true;

        bool foundDNA = false;
        for (const clang::CXXBaseSpecifier& base : decl->bases())
        {
            llvm::outs() << "BASE " << base.getType().getCanonicalType().getAsString() << "\n";
            if (!base.getType().getCanonicalType().getAsString().compare("struct Athena::io::DNA"))
            {
                foundDNA = true;
                break;
            }
        }
        if (!foundDNA)
            return true;

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
                llvm::outs() << "    Alias " << tsType->getTemplateName().getAsTemplateDecl()->getNameAsString();
                llvm::outs() << " " << tsType->getNumArgs() << " ";
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

class FindNamedClassConsumer : public clang::ASTConsumer
{
    FindNamedClassVisitor visitor;
public:
    explicit FindNamedClassConsumer(clang::ASTContext& context)
    : visitor(context) {}
    void HandleTranslationUnit(clang::ASTContext& context)
    {visitor.TraverseDecl(context.getTranslationUnitDecl());}
};

static const char ATDNA_PREAMBLE[] =
"#include <stdint.h>\n"
"typedef uint16_t atUint16;\n"
"typedef int16_t atInt16;\n"
"typedef uint32_t atUint32;\n"
"typedef int32_t atInt32;\n"
"typedef uint64_t atUint64;\n"
"typedef int64_t atInt64;\n"
"#define TYPES_HPP\n";

class FindNamedClassAction : public clang::ASTFrontendAction
{
    std::unique_ptr<llvm::MemoryBuffer> preambleBuf;
public:
    explicit FindNamedClassAction()
    : preambleBuf(llvm::MemoryBuffer::getMemBuffer(ATDNA_PREAMBLE, "ATDNA_PREAMBLE.hpp")) {}
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler,
                                                          llvm::StringRef /*filename*/)
    {
        //compiler.getDiagnostics().setSuppressAllDiagnostics();
        compiler.getPreprocessorOpts().addRemappedFile("ATDNA_PREAMBLE.hpp", preambleBuf.get());
        compiler.getPreprocessorOpts().ChainedIncludes.push_back("ATDNA_PREAMBLE.hpp");
        compiler.getPreprocessorOpts().RetainRemappedFileBuffers = true;
        return std::unique_ptr<clang::ASTConsumer>(new FindNamedClassConsumer(compiler.getASTContext()));
    }
};

int main(int argc, const char** argv)
{
    if (argc > 1)
    {
        clang::FileManager fman((clang::FileSystemOptions()));
        llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buf = fman.getBufferForFile(argv[1]);
        std::error_code ec;
        if ((ec = buf.getError()))
            throw ec;
        llvm::Twine buft(buf->get()->getBuffer());
        clang::tooling::runToolOnCodeWithArgs(new FindNamedClassAction, buft, {"-std=c++11"}, argv[1]);
    }
    return 0;
}

