#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/Version.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/FrontendAction.h>
#include <clang/Frontend/Utils.h>
#include <clang/Sema/Sema.h>
#include <clang/Tooling/Tooling.h>

#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Format.h>

using namespace std::literals;

static unsigned AthenaError = 0;
#define ATHENA_DNA_BASETYPE "struct athena::io::DNA"

#ifndef INSTALL_PREFIX
#define INSTALL_PREFIX / usr / local
#endif
#define XSTR(s) STR(s)
#define STR(s) #s

static llvm::cl::opt<bool> Help("h", llvm::cl::desc("Alias for -help"), llvm::cl::Hidden);

static llvm::cl::opt<bool> Verbose("v", llvm::cl::desc("verbose mode"));

static llvm::cl::OptionCategory ATDNAFormatCategory("atdna options");

static llvm::cl::opt<std::string> OutputFilename("o", llvm::cl::desc("Specify output filename"),
                                                 llvm::cl::value_desc("filename"), llvm::cl::Prefix);

static llvm::cl::opt<bool> FExceptions("fexceptions", llvm::cl::desc("Enable C++ Exceptions"));
static llvm::cl::opt<bool> FMSCompat("fms-compatibility", llvm::cl::desc("Enable MS header compatibility"));
static llvm::cl::opt<std::string>
    FMSCompatVersion("fms-compatibility-version",
                     llvm::cl::desc("Specify MS compatibility version (18.00 for VS2013, 19.00 for VS2015)"));

static llvm::cl::list<std::string> InputFilenames(llvm::cl::Positional, llvm::cl::desc("<Input files>"),
                                                  llvm::cl::ZeroOrMore);

static llvm::cl::list<std::string> IncludeSearchPaths("I", llvm::cl::desc("Header search path"), llvm::cl::Prefix);

static llvm::cl::list<std::string> SystemIncludeSearchPaths("isystem", llvm::cl::desc("System Header search path"));

static llvm::cl::list<std::string> StandardCXXLibISystem("stdlib++-isystem",
                                                         llvm::cl::desc("Standard C++ library search path"));

static llvm::cl::opt<std::string> StandardCXXLib("stdlib", llvm::cl::desc("Standard C++ library"));

static llvm::cl::opt<bool> DepFile("MD", llvm::cl::desc("Make Dependency file"));

static llvm::cl::opt<std::string> DepFileOut("MF", llvm::cl::desc("Dependency file out path"));

static llvm::cl::list<std::string> DepFileTargets("MT", llvm::cl::desc("Dependency file targets"));

static llvm::cl::list<std::string> SystemIncRoot("isysroot", llvm::cl::desc("System include root"));

static llvm::cl::list<std::string> PreprocessorDefines("D", llvm::cl::desc("Preprocessor define"), llvm::cl::Prefix);

static llvm::cl::opt<bool> EmitIncludes("emit-includes",
                                        llvm::cl::desc("Emit DNA for included files (not just main file)"));

/* LLVM 3.7 changed the stream type */
#if LLVM_VERSION_MAJOR > 3 || (LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR >= 7)
using StreamOut = llvm::raw_pwrite_stream;
#else
using StreamOut = llvm::raw_fd_ostream;
#endif

#if LLVM_VERSION_MAJOR >= 12
static inline bool GetIntegerConstantExpr(const clang::Expr* expr, llvm::APSInt& out, const clang::ASTContext& ctx) {
  const auto optional = expr->getIntegerConstantExpr(ctx);
  if (optional) {
    out = optional.getValue();
    return true;
  }
  return false;
}
#else
static inline bool GetIntegerConstantExpr(const clang::Expr* expr, llvm::APSInt& out, const clang::ASTContext& ctx) {
  return expr->isIntegerConstantExpr(out, ctx);
}
#endif

class ATDNAEmitVisitor : public clang::RecursiveASTVisitor<ATDNAEmitVisitor> {
  clang::ASTContext& context;
  StreamOut& fileOut;

  bool isDNARecord(const clang::CXXRecordDecl* record, std::string& baseDNA) {
    for (const clang::CXXBaseSpecifier& base : record->bases()) {
      const clang::QualType qtp = base.getType().getCanonicalType();
      if (!qtp.getAsString().compare(0, sizeof(ATHENA_DNA_BASETYPE) - 1, ATHENA_DNA_BASETYPE))
        return true;
    }
    for (const clang::CXXBaseSpecifier& base : record->bases()) {
      clang::QualType qtp = base.getType().getCanonicalType();
      const clang::Type* tp = qtp.getTypePtrOrNull();
      if (tp) {
        const clang::CXXRecordDecl* rDecl = tp->getAsCXXRecordDecl();
        if (rDecl) {
          if (isDNARecord(rDecl, baseDNA)) {
            bool hasRead = false;
            bool hasWrite = false;
            for (const clang::CXXMethodDecl* method : rDecl->methods()) {
              std::string compName = method->getDeclName().getAsString();
              if (compName == "read")
                hasRead = true;
              else if (compName == "write")
                hasWrite = true;
            }
            if (hasRead && hasWrite) {
              std::string templateStmt;
              GetNestedTypeName(rDecl, templateStmt, baseDNA);
            }
            return true;
          }
        }
      }
    }
    return false;
  }

  int64_t GetSizeValue(const clang::Type* theType, unsigned width) {
    if (theType->isEnumeralType()) {
      const auto* eType = static_cast<const clang::EnumType*>(theType);
      clang::EnumDecl* eDecl = eType->getDecl();
      theType = eDecl->getIntegerType().getCanonicalType().getTypePtr();

      const auto* bType = static_cast<const clang::BuiltinType*>(theType);
      if (bType->isBooleanType()) {
        return 1;
      } else if (bType->isUnsignedInteger() || bType->isSignedInteger()) {
        return width / 8;
      }
    } else if (theType->isBuiltinType()) {
      const auto* bType = static_cast<const clang::BuiltinType*>(theType);
      if (bType->isBooleanType()) {
        return 1;
      } else if (bType->isUnsignedInteger() || bType->isSignedInteger() || bType->isFloatingPoint()) {
        return width / 8;
      }
    } else if (theType->isRecordType()) {
      const clang::CXXRecordDecl* rDecl = theType->getAsCXXRecordDecl();
      for (const clang::FieldDecl* field : rDecl->fields()) {
        if (field->getName() == "clangVec") {
          const auto* vType = static_cast<const clang::VectorType*>(field->getType().getTypePtr());
          if (vType->isVectorType()) {
            const auto* eType = static_cast<const clang::BuiltinType*>(vType->getElementType().getTypePtr());
            const uint64_t typeWidth = context.getTypeInfo(eType).Width;
            if (!eType->isBuiltinType() || !eType->isFloatingPoint() || (typeWidth != 32 && typeWidth != 64))
              continue;
            if (vType->getNumElements() == 2) {
              return typeWidth / 8 * 2;
            } else if (vType->getNumElements() == 3) {
              return typeWidth / 8 * 3;
            } else if (vType->getNumElements() == 4) {
              return typeWidth / 8 * 4;
            }
          }
        }
      }
    }
    return 0;
  }

  static std::string GetFieldString(const std::string& fieldName) {
#if 0
    size_t underscorePos = fieldName.find('_');
    std::string idString = fieldName;
    if (underscorePos != std::string::npos && underscorePos != fieldName.size() - 1)
      idString.assign(fieldName.begin() + underscorePos + 1, fieldName.end());
    return idString;
#endif
    return fieldName;
  }

  static std::string GetPropIdExpr(const clang::FieldDecl* field, const std::string& fieldName) {
    std::string fieldStr = GetFieldString(fieldName);
    std::string propIdExpr = "\""s.append(fieldStr).append("\"sv");
    for (clang::Attr* attr : field->attrs()) {
      if (clang::AnnotateAttr* annot = clang::dyn_cast_or_null<clang::AnnotateAttr>(attr)) {
        llvm::StringRef textRef = annot->getAnnotation();
#if LLVM_VERSION_MAJOR >= 13
        if (textRef.startswith_insensitive("rcrc32="))
#else
        if (textRef.startswith_lower("rcrc32="))
#endif
        {
          unsigned long num = strtoul(textRef.data() + 7, nullptr, 16);
          std::string tmpS;
          llvm::raw_string_ostream s(tmpS);
          s << llvm::format("\"%s\"sv, 0x%08X", fieldStr.c_str(), num);
          propIdExpr = s.str();
          break;
        }
      }
    }
    return propIdExpr;
  }

  static std::string GetOpString(const std::string& fieldName, const std::string& propIdExpr,
                                 const std::string& endianExpr) {

    return "<Op, "s.append(endianExpr)
        .append(">(athena::io::PropId(")
        .append(propIdExpr)
        .append("), ")
        .append(fieldName)
        .append(", s)");
  }

  static std::string GetOpString(const std::string& fieldName, const std::string& propIdExpr) {

    return "<Op>(athena::io::PropId("s.append(propIdExpr).append("), ").append(fieldName).append(", s)");
  }

  static std::string GetVectorOpString(const std::string& fieldName, const std::string& propIdExpr,
                                       const std::string& sizeExpr, const std::string& endianExpr) {
    return "<Op, "s.append(endianExpr)
        .append(">(athena::io::PropId(")
        .append(propIdExpr)
        .append("), ")
        .append(fieldName)
        .append(", ")
        .append(sizeExpr)
        .append(", s)");
  }

  static std::string GetVectorOpString(const std::string& fieldName, const std::string& propIdExpr,
                                       const std::string& sizeExpr) {
    return "<Op>(athena::io::PropId("s.append(propIdExpr)
        .append("), ")
        .append(fieldName)
        .append(", ")
        .append(sizeExpr)
        .append(", s)");
  }

  void RecurseNestedTypeName(const clang::DeclContext* decl, std::string& templateStmt, std::string& qualType) {
    if (!decl)
      return;
    RecurseNestedTypeName(decl->getParent(), templateStmt, qualType);
    if (const clang::CXXRecordDecl* rec = clang::dyn_cast_or_null<clang::CXXRecordDecl>(decl)) {
      if (!qualType.empty())
        qualType += "::";
      qualType += rec->getName();
      const clang::ClassTemplateSpecializationDecl* cts =
          clang::dyn_cast_or_null<clang::ClassTemplateSpecializationDecl>(rec);
      if (cts && cts->isExplicitSpecialization()) {
        qualType += '<';
        bool needsComma = false;
        for (auto& arg : cts->getTemplateArgs().asArray()) {
          if (needsComma)
            qualType += ", ";
          llvm::raw_string_ostream OS(qualType);
#if LLVM_VERSION_MAJOR >= 13
          arg.print(context.getPrintingPolicy(), OS, false);
#else
          arg.print(context.getPrintingPolicy(), OS);
#endif
          needsComma = true;
        }
        qualType += '>';
      } else if (const clang::ClassTemplateDecl* ct = rec->getDescribedClassTemplate()) {
        templateStmt += "template <";
        qualType += '<';
        bool needsComma = false;
        for (const clang::NamedDecl* parm : *ct->getTemplateParameters()) {
          if (const clang::TemplateTypeParmDecl* tpParm = clang::dyn_cast_or_null<clang::TemplateTypeParmDecl>(parm)) {
            if (needsComma) {
              templateStmt += ", ";
              qualType += ", ";
            }
            templateStmt += "class "s.append(tpParm->getName().str());
            qualType += tpParm->getName();
            needsComma = true;
          } else if (const clang::NonTypeTemplateParmDecl* nonTypeParm =
                         clang::dyn_cast_or_null<clang::NonTypeTemplateParmDecl>(parm)) {
            if (needsComma) {
              templateStmt += ", ";
              qualType += ", ";
            }
            templateStmt += nonTypeParm->getType()
                                .getAsString(context.getPrintingPolicy())
                                .append(1, ' ')
                                .append(nonTypeParm->getName().str());
            qualType += nonTypeParm->getName();
            needsComma = true;
          }
        }
        templateStmt += ">\n";
        qualType += '>';
      }
    } else if (const clang::NamedDecl* namedDecl = clang::dyn_cast_or_null<clang::NamedDecl>(decl)) {
      if (!qualType.empty())
        qualType += "::";
      qualType += namedDecl->getName();
    }
  }

  void GetNestedTypeName(const clang::DeclContext* decl, std::string& templateStmt, std::string& qualType) {
    templateStmt.clear();
    qualType.clear();
    RecurseNestedTypeName(decl, templateStmt, qualType);
  }

  void RecurseNestedTypeSpecializations(const clang::DeclContext* decl,
                                        std::vector<std::pair<std::string, int>>& specializations) {
    if (!decl) {
      specializations.emplace_back();
      return;
    }

    std::vector<std::pair<std::string, int>> parentSpecializations;
    RecurseNestedTypeSpecializations(decl->getParent(), parentSpecializations);
    bool foundSpecializations = false;
    if (const clang::CXXRecordDecl* rec = clang::dyn_cast_or_null<clang::CXXRecordDecl>(decl)) {
      const clang::ClassTemplateSpecializationDecl* cts =
          clang::dyn_cast_or_null<clang::ClassTemplateSpecializationDecl>(rec);
      if (cts && cts->isExplicitSpecialization()) {
        for (const auto& parent : parentSpecializations) {
          if (parent.first.empty()) {
            specializations.emplace_back(std::string(rec->getName().str()).append(1, '<'), 0);
          } else {
            auto specialization = std::string(parent.first).append("::").append(rec->getName().str()).append(1, '<');
            specializations.emplace_back(std::move(specialization), parent.second);
          }
          bool needsComma = false;
          for (auto& arg : cts->getTemplateArgs().asArray()) {
            if (needsComma)
              specializations.back().first += ", ";
            llvm::raw_string_ostream OS(specializations.back().first);
#if LLVM_VERSION_MAJOR >= 13
            arg.print(context.getPrintingPolicy(), OS, false);
#else
            arg.print(context.getPrintingPolicy(), OS);
#endif
            needsComma = true;
          }
          specializations.back().first += '>';
        }
        foundSpecializations = true;
      } else if (const clang::ClassTemplateDecl* ct = rec->getDescribedClassTemplate()) {
        int numParms = 0;
        for (const clang::NamedDecl* parm : *ct->getTemplateParameters())
          if (clang::dyn_cast_or_null<clang::TemplateTypeParmDecl>(parm) ||
              clang::dyn_cast_or_null<clang::NonTypeTemplateParmDecl>(parm))
            ++numParms;

        for (clang::Attr* attr : rec->attrs()) {
          if (clang::AnnotateAttr* annot = clang::dyn_cast_or_null<clang::AnnotateAttr>(attr)) {
            llvm::StringRef textRef = annot->getAnnotation();
#if LLVM_VERSION_MAJOR >= 13
            if (textRef.startswith_insensitive("specparms="))
#else
            if (textRef.startswith_lower("specparms="))
#endif
            {
              llvm::SmallVector<llvm::StringRef, 16> specParms;
              textRef.substr(10).split(specParms, ',');
              int numTuples = int(specParms.size()) / numParms;
              for (const auto& parent : parentSpecializations) {
                for (int i = 0; i < numTuples; ++i) {
                  if (parent.first.empty()) {
                    specializations.emplace_back(std::string(rec->getName().str()).append(1, '<'), 1);
                  } else {
                    auto specialization =
                        std::string(parent.first).append("::").append(rec->getName().str()).append(1, '<');
                    specializations.emplace_back(std::move(specialization), parent.second + 1);
                  }
                  bool needsComma = false;
                  for (auto it = specParms.begin() + i * numParms;
                       it != specParms.end() && it != specParms.begin() + (i + 1) * numParms; ++it) {
                    llvm::StringRef trimmed = it->trim();
                    if (needsComma)
                      specializations.back().first += ", ";
                    specializations.back().first += trimmed;
                    needsComma = true;
                  }
                  specializations.back().first += '>';
                }
              }
              foundSpecializations = true;
              break;
            }
          }
        }
      }
    }

    if (!foundSpecializations) {
      for (const auto& parent : parentSpecializations) {
        if (const clang::NamedDecl* namedDecl = clang::dyn_cast_or_null<clang::NamedDecl>(decl)) {
          if (parent.first.empty()) {
            specializations.emplace_back(namedDecl->getName().str(), parent.second);
          } else {
            specializations.emplace_back(std::string(parent.first).append("::").append(namedDecl->getName().str()),
                                         parent.second);
          }
        } else {
          specializations.push_back(parent);
        }
      }
    }
  }

  std::vector<std::pair<std::string, int>> GetNestedTypeSpecializations(const clang::DeclContext* decl) {
    std::vector<std::pair<std::string, int>> ret;
    RecurseNestedTypeSpecializations(decl, ret);
    return ret;
  }

  void emitEnumerateFunc(clang::CXXRecordDecl* decl, const std::string& baseDNA) {
    std::string templateStmt;
    std::string qualTypeStr;
    GetNestedTypeName(decl, templateStmt, qualTypeStr);

    fileOut << templateStmt;
    fileOut << "template <class Op>\nvoid " << qualTypeStr << "::Enumerate(typename Op::StreamT& s) {\n";

    if (baseDNA.size())
      fileOut << "  " << baseDNA << "::Enumerate<Op>(s);\n";

    enum class NodeType { Do, DoSeek, DoAlign };
    struct OutputNode {
      NodeType m_type = NodeType::Do;
      std::string m_fieldName;
      std::string m_ioOp;
      bool m_squelched = false;
      OutputNode(NodeType type, std::string fieldName, std::string ioOp, bool squelched)
      : m_type(type), m_fieldName(std::move(fieldName)), m_ioOp(std::move(ioOp)), m_squelched(squelched) {}
    };
    std::vector<OutputNode> outputNodes;

    for (const clang::FieldDecl* field : decl->fields()) {
      clang::QualType qualType = field->getType();
      const clang::Type* regType = qualType.getTypePtrOrNull();
      if (!regType || regType->getTypeClass() == clang::Type::TemplateTypeParm)
        continue;
      while (regType->getTypeClass() == clang::Type::Elaborated || regType->getTypeClass() == clang::Type::Typedef)
        regType = regType->getUnqualifiedDesugaredType();

      /* Resolve constant array */
      while (regType->getTypeClass() == clang::Type::ConstantArray) {
        const auto* caType = static_cast<const clang::ConstantArrayType*>(regType);
        qualType = caType->getElementType();
        regType = qualType.getTypePtrOrNull();
        if (regType->getTypeClass() == clang::Type::Elaborated)
          regType = regType->getUnqualifiedDesugaredType();
      }

      std::string fieldName = field->getName().str();
      std::string propIdExpr = GetPropIdExpr(field, fieldName);

      if (regType->getTypeClass() == clang::Type::TemplateSpecialization) {
        const auto* tsType = static_cast<const clang::TemplateSpecializationType*>(regType);
        const clang::TemplateDecl* tsDecl = tsType->getTemplateName().getAsTemplateDecl();
        const clang::TemplateParameterList* classParms = tsDecl->getTemplateParameters();

        if (tsDecl->getName() == "Value") {
          llvm::APSInt endian(64, -1);
          std::string endianExprStr;
          bool defaultEndian = true;
          /* FIXME: The is the following code required? */
#if 0
          if (classParms->size() >= 2) {
            const clang::NamedDecl* endianParm = classParms->getParam(1);
            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm) {
              const auto* nttParm = static_cast<const clang::NonTypeTemplateParmDecl*>(endianParm);
              llvm::raw_string_ostream strStream(endianExprStr);
              nttParm->print(strStream, context.getPrintingPolicy());
            }
          }
#endif
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const clang::Expr* expr = arg.getAsExpr();
              llvm::raw_string_ostream strStream(endianExprStr);
              expr->printPretty(strStream, nullptr, context.getPrintingPolicy());
              defaultEndian = false;
            }
          }

          std::string ioOp;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Type) {
              if (defaultEndian) {
                ioOp = GetOpString(fieldName, propIdExpr);
              } else {
                ioOp = GetOpString(fieldName, propIdExpr, endianExprStr);
              }
            }
          }

          if (ioOp.empty()) {
            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocation(), AthenaError);
            diag.AddString("Unable to use type '"s.append(tsDecl->getName().str()).append("' with Athena"));
            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
            continue;
          }

          outputNodes.emplace_back(NodeType::Do, std::move(fieldName), std::move(ioOp), false);
        } else if (tsDecl->getName() == "Vector") {
          llvm::APSInt endian(64, -1);
          std::string endianExprStr;
          bool defaultEndian = true;
          if (classParms->size() >= 3) {
            const clang::NamedDecl* endianParm = classParms->getParam(2);
            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm) {
              const auto* nttParm = static_cast<const clang::NonTypeTemplateParmDecl*>(endianParm);
              llvm::raw_string_ostream strStream(endianExprStr);
              nttParm->print(strStream, context.getPrintingPolicy());
            }
          }

          std::string sizeExpr;
          size_t idx = 0;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
              if (idx == 1) {
                const auto* uExpr = static_cast<const clang::UnaryExprOrTypeTraitExpr*>(expr);
                if (uExpr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                    uExpr->getKind() == clang::UETT_SizeOf) {
                  const clang::Expr* argExpr = uExpr->getArgumentExpr();
                  while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                    argExpr = static_cast<const clang::ParenExpr*>(argExpr)->getSubExpr();

                  if (argExpr->getStmtClass() == clang::Stmt::DeclRefExprClass) {
                    const auto* drExpr = static_cast<const clang::DeclRefExpr*>(argExpr);
                    std::string testName = drExpr->getFoundDecl()->getNameAsString();
                    for (auto i = outputNodes.rbegin(); i != outputNodes.rend(); ++i) {
                      if (i->m_fieldName == testName) {
                        i->m_squelched = true;
                        break;
                      }
                    }
                  }
                  llvm::raw_string_ostream strStream2(sizeExpr);
                  argExpr->printPretty(strStream2, nullptr, context.getPrintingPolicy());
                }
              } else if (idx == 2) {
                defaultEndian = false;
                llvm::raw_string_ostream strStream(endianExprStr);
                expr->printPretty(strStream, nullptr, context.getPrintingPolicy());
              }
            }
            ++idx;
          }

          clang::QualType templateType;
          std::string ioOp;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Type) {
              templateType = arg.getAsType().getCanonicalType();
              if (defaultEndian) {
                ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExpr);
              } else {
                ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExpr, endianExprStr);
              }
            }
          }

          if (ioOp.empty()) {
            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocation(), AthenaError);
            diag.AddString("Unable to use type '"s.append(templateType.getAsString()).append("' with Athena"));
            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
            continue;
          }

          if (sizeExpr.empty()) {
            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocation(), AthenaError);
            diag.AddString("Unable to use count variable with Athena");
            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
            continue;
          }

          outputNodes.emplace_back(NodeType::Do, std::move(fieldName), std::move(ioOp), false);
        } else if (tsDecl->getName() == "Buffer") {
          const clang::Expr* sizeExpr = nullptr;
          std::string sizeExprStr;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const auto* uExpr =
                  static_cast<const clang::UnaryExprOrTypeTraitExpr*>(arg.getAsExpr()->IgnoreImpCasts());
              if (uExpr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                  uExpr->getKind() == clang::UETT_SizeOf) {
                const clang::Expr* argExpr = uExpr->getArgumentExpr();
                while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                  argExpr = static_cast<const clang::ParenExpr*>(argExpr)->getSubExpr();
                sizeExpr = argExpr;
                llvm::raw_string_ostream strStream(sizeExprStr);
                argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
              }
            }
          }
          if (sizeExprStr.empty()) {
            if (sizeExpr) {
              clang::DiagnosticBuilder diag = context.getDiagnostics().Report(sizeExpr->getExprLoc(), AthenaError);
              diag.AddString("Unable to use size variable with Athena");
              diag.AddSourceRange(clang::CharSourceRange(sizeExpr->getSourceRange(), true));
            } else {
              clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocation(), AthenaError);
              diag.AddString("Unable to use size variable with Athena");
              diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
            }
            continue;
          }

          std::string ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr);
          outputNodes.emplace_back(NodeType::Do, std::move(fieldName), std::move(ioOp), false);
        } else if (tsDecl->getName() == "String") {
          std::string sizeExprStr;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
              const auto* uExpr = static_cast<const clang::UnaryExprOrTypeTraitExpr*>(expr);
              llvm::APSInt sizeLiteral;
              if (expr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                  uExpr->getKind() == clang::UETT_SizeOf) {
                const clang::Expr* argExpr = uExpr->getArgumentExpr();
                while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                  argExpr = static_cast<const clang::ParenExpr*>(argExpr)->getSubExpr();
                llvm::raw_string_ostream strStream(sizeExprStr);
                argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
              } else if (GetIntegerConstantExpr(expr, sizeLiteral, context)) {
#if LLVM_VERSION_MAJOR >= 13
                llvm::SmallVector<char> strStream;
                sizeLiteral.toString(strStream, 10);
                sizeExprStr = std::string(strStream.begin(), strStream.end());
#else
                sizeExprStr = sizeLiteral.toString(10);
#endif
              }
            }
          }

          std::string ioOp;
          if (!sizeExprStr.empty()) {
            ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr);
          } else {
            ioOp = GetOpString(fieldName, propIdExpr);
          }

          outputNodes.emplace_back(NodeType::Do, std::move(fieldName), std::move(ioOp), false);
        } else if (tsDecl->getName() == "WString") {
          llvm::APSInt endian(64, -1);
          std::string endianExprStr;
          bool defaultEndian = true;
          if (classParms->size() >= 2) {
            const clang::NamedDecl* endianParm = classParms->getParam(1);
            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm) {
              const auto* nttParm = static_cast<const clang::NonTypeTemplateParmDecl*>(endianParm);
              llvm::raw_string_ostream strStream(endianExprStr);
              nttParm->print(strStream, context.getPrintingPolicy());
            }
          }

          std::string sizeExprStr;
          size_t idx = 0;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
              if (idx == 0) {
                llvm::APSInt sizeLiteral;
                const auto* uExpr = static_cast<const clang::UnaryExprOrTypeTraitExpr*>(expr);
                if (expr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                    uExpr->getKind() == clang::UETT_SizeOf) {
                  const clang::Expr* argExpr = uExpr->getArgumentExpr();
                  while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                    argExpr = static_cast<const clang::ParenExpr*>(argExpr)->getSubExpr();
                  llvm::raw_string_ostream strStream2(sizeExprStr);
                  argExpr->printPretty(strStream2, nullptr, context.getPrintingPolicy());
                } else if (GetIntegerConstantExpr(expr, sizeLiteral, context)) {
#if LLVM_VERSION_MAJOR >= 13
                  llvm::SmallVector<char> strStream;
                  sizeLiteral.toString(strStream, 10);
                  sizeExprStr = std::string(strStream.begin(), strStream.end());
#else
                  sizeExprStr = sizeLiteral.toString(10);
#endif
                }
              } else if (idx == 1) {
                defaultEndian = false;
                llvm::raw_string_ostream strStream(endianExprStr);
                expr->printPretty(strStream, nullptr, context.getPrintingPolicy());
              }
            }
            ++idx;
          }

          std::string ioOp;
          if (!sizeExprStr.empty()) {
            if (defaultEndian) {
              ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr);
            } else {
              ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr, endianExprStr);
            }
          } else {
            if (defaultEndian) {
              ioOp = GetOpString(fieldName, propIdExpr);
            } else {
              ioOp = GetOpString(fieldName, propIdExpr, endianExprStr);
            }
          }

          outputNodes.emplace_back(NodeType::Do, std::move(fieldName), std::move(ioOp), false);
        } else if (tsDecl->getName() == "Seek") {
          size_t idx = 0;
          std::string offsetExprStr;
          llvm::APSInt direction(64, false);
          const clang::Expr* directionExpr = nullptr;
          bool bad = false;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
              if (!idx) {
                const auto* uExpr = static_cast<const clang::UnaryExprOrTypeTraitExpr*>(expr);
                llvm::APSInt offsetLiteral;
                if (expr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                    uExpr->getKind() == clang::UETT_SizeOf) {
                  const clang::Expr* argExpr = uExpr->getArgumentExpr();
                  while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                    argExpr = static_cast<const clang::ParenExpr*>(argExpr)->getSubExpr();
                  llvm::raw_string_ostream strStream(offsetExprStr);
                  argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                } else if (GetIntegerConstantExpr(expr, offsetLiteral, context)) {
#if LLVM_VERSION_MAJOR >= 13
                  llvm::SmallVector<char> strStream;
                  offsetLiteral.toString(strStream, 10);
                  offsetExprStr = std::string(strStream.begin(), strStream.end());
#else
                  offsetExprStr = offsetLiteral.toString(10);
#endif
                }
              } else {
                clang::APValue result;
                if (expr->isCXX11ConstantExpr(context, &result)) {
                  directionExpr = expr;
                  direction = result.getInt();
                } else {
                  clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getExprLoc(), AthenaError);
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
          if (directionVal < 0 || directionVal > 2) {
            if (directionExpr) {
              clang::DiagnosticBuilder diag = context.getDiagnostics().Report(directionExpr->getExprLoc(), AthenaError);
              diag.AddString("Direction parameter must be 'Begin', 'Current', or 'End'");
              diag.AddSourceRange(clang::CharSourceRange(directionExpr->getSourceRange(), true));
            } else {
              clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocation(), AthenaError);
              diag.AddString("Direction parameter must be 'Begin', 'Current', or 'End'");
              diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
            }
            continue;
          }

          if (directionVal == 0) {
            outputNodes.emplace_back(NodeType::DoSeek, std::move(fieldName),
                                     "<Op>("s.append(offsetExprStr).append(", athena::SeekOrigin::Begin, s)"), false);
          } else if (directionVal == 1) {
            outputNodes.emplace_back(NodeType::DoSeek, std::move(fieldName),
                                     "<Op>("s.append(offsetExprStr).append(", athena::SeekOrigin::Current, s)"), false);
          } else if (directionVal == 2) {
            outputNodes.emplace_back(NodeType::DoSeek, std::move(fieldName),
                                     "<Op>("s.append(offsetExprStr).append(", athena::SeekOrigin::End, s)"), false);
          }
        } else if (tsDecl->getName() == "Align") {
          llvm::APSInt align(64, 0);
          bool bad = false;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const clang::Expr* expr = arg.getAsExpr();
              if (!GetIntegerConstantExpr(expr, align, context)) {
                clang::DiagnosticBuilder diag = context.getDiagnostics().Report(expr->getExprLoc(), AthenaError);
                diag.AddString("Unable to use non-constant align expression in Athena");
                diag.AddSourceRange(clang::CharSourceRange(expr->getSourceRange(), true));
                bad = true;
                break;
              }
            }
          }
          if (bad)
            continue;

          const int64_t alignVal = align.getSExtValue();
          if (alignVal) {
#if LLVM_VERSION_MAJOR >= 13
            llvm::SmallVector<char> alignStream;
            align.toString(alignStream, 10, true);
            outputNodes.emplace_back(NodeType::DoAlign, std::move(fieldName),
                                     "<Op>("s.append(alignStream.begin(), alignStream.end()).append(", s)"), false);
#else
            outputNodes.emplace_back(NodeType::DoAlign, std::move(fieldName),
                                     "<Op>("s.append(align.toString(10, true)).append(", s)"), false);
#endif
          }
        } else {
          const clang::NamedDecl* nd = tsDecl->getTemplatedDecl();
          if (const clang::CXXRecordDecl* rd = clang::dyn_cast_or_null<clang::CXXRecordDecl>(nd)) {
            std::string baseDNA2;
            if (isDNARecord(rd, baseDNA2)) {
              outputNodes.emplace_back(NodeType::Do, std::move(fieldName), GetOpString(fieldName, propIdExpr), false);
            }
          }
        }
      } else if (regType->getTypeClass() == clang::Type::Record) {
        const clang::CXXRecordDecl* cxxRDecl = regType->getAsCXXRecordDecl();
        std::string baseDNA2;
        if (cxxRDecl && isDNARecord(cxxRDecl, baseDNA2)) {
          outputNodes.emplace_back(NodeType::Do, std::move(fieldName), GetOpString(fieldName, propIdExpr), false);
        }
      }
    }

    for (const OutputNode& node : outputNodes) {
      switch (node.m_type) {
      case NodeType::Do:
        if (node.m_squelched)
          fileOut << "  DoSize" << node.m_ioOp << ";\n";
        else
          fileOut << "  Do" << node.m_ioOp << ";\n";
        break;
      case NodeType::DoSeek:
        fileOut << "  DoSeek" << node.m_ioOp << ";\n";
        break;
      case NodeType::DoAlign:
        fileOut << "  DoAlign" << node.m_ioOp << ";\n";
        break;
      }
    }

    fileOut << "}\n\n";
  }

  void emitLookupFunc(clang::CXXRecordDecl* decl, const std::string& baseDNA) {
    std::string templateStmt;
    std::string qualTypeStr;
    GetNestedTypeName(decl, templateStmt, qualTypeStr);

    fileOut << templateStmt;
    fileOut << "template <class Op>\nbool " << qualTypeStr << "::Lookup(uint64_t hash, typename Op::StreamT& s) {\n";

    if (baseDNA.size())
      fileOut << "  if (" << baseDNA << "::Lookup<Op>(hash, s))\n"
              << "    return true;\n";

    fileOut << "  switch (hash) {\n";

    for (const clang::FieldDecl* field : decl->fields()) {
      clang::QualType qualType = field->getType();
      const clang::Type* regType = qualType.getTypePtrOrNull();
      if (!regType || regType->getTypeClass() == clang::Type::TemplateTypeParm)
        continue;
      while (regType->getTypeClass() == clang::Type::Elaborated || regType->getTypeClass() == clang::Type::Typedef)
        regType = regType->getUnqualifiedDesugaredType();

      /* Resolve constant array */
      while (regType->getTypeClass() == clang::Type::ConstantArray) {
        const auto* caType = static_cast<const clang::ConstantArrayType*>(regType);
        qualType = caType->getElementType();
        regType = qualType.getTypePtrOrNull();
        if (regType->getTypeClass() == clang::Type::Elaborated)
          regType = regType->getUnqualifiedDesugaredType();
      }

      std::string fieldName = field->getName().str();
      std::string propIdExpr = GetPropIdExpr(field, fieldName);

      if (regType->getTypeClass() == clang::Type::TemplateSpecialization) {
        const auto* tsType = static_cast<const clang::TemplateSpecializationType*>(regType);
        const clang::TemplateDecl* tsDecl = tsType->getTemplateName().getAsTemplateDecl();
        const clang::TemplateParameterList* classParms = tsDecl->getTemplateParameters();

        if (tsDecl->getName() == "Value") {
          llvm::APSInt endian(64, -1);
          std::string endianExprStr;
          bool defaultEndian = true;
          if (classParms->size() >= 2) {
            const clang::NamedDecl* endianParm = classParms->getParam(1);
            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm) {
              const auto* nttParm = static_cast<const clang::NonTypeTemplateParmDecl*>(endianParm);
              llvm::raw_string_ostream strStream(endianExprStr);
              nttParm->print(strStream, context.getPrintingPolicy());
            }
          }

          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const clang::Expr* expr = arg.getAsExpr();
              llvm::raw_string_ostream strStream(endianExprStr);
              expr->printPretty(strStream, nullptr, context.getPrintingPolicy());
              defaultEndian = false;
            }
          }

          std::string ioOp;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Type) {
              if (defaultEndian)
                ioOp = GetOpString(fieldName, propIdExpr);
              else
                ioOp = GetOpString(fieldName, propIdExpr, endianExprStr);
            }
          }

          if (ioOp.empty()) {
            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocation(), AthenaError);
            diag.AddString("Unable to use type '"s.append(tsDecl->getName().str()).append("' with Athena"));
            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
            continue;
          }

          fileOut << "  AT_PROP_CASE(" << propIdExpr << "):\n"
                  << "    Do" << ioOp << ";\n"
                  << "    return true;\n";

        } else if (tsDecl->getName() == "Vector") {
          llvm::APSInt endian(64, -1);
          std::string endianExprStr;
          bool defaultEndian = true;
          if (classParms->size() >= 3) {
            const clang::NamedDecl* endianParm = classParms->getParam(2);
            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm) {
              const auto* nttParm = static_cast<const clang::NonTypeTemplateParmDecl*>(endianParm);
              llvm::raw_string_ostream strStream(endianExprStr);
              nttParm->print(strStream, context.getPrintingPolicy());
            }
          }

          std::string sizeExpr;
          size_t idx = 0;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
              if (idx == 1) {
                const auto* uExpr = static_cast<const clang::UnaryExprOrTypeTraitExpr*>(expr);
                if (uExpr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                    uExpr->getKind() == clang::UETT_SizeOf) {
                  const clang::Expr* argExpr = uExpr->getArgumentExpr();
                  while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                    argExpr = static_cast<const clang::ParenExpr*>(argExpr)->getSubExpr();
                  llvm::raw_string_ostream strStream2(sizeExpr);
                  argExpr->printPretty(strStream2, nullptr, context.getPrintingPolicy());
                }
              } else if (idx == 2) {
                llvm::raw_string_ostream strStream(endianExprStr);
                expr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                defaultEndian = false;
              }
            }
            ++idx;
          }

          clang::QualType templateType;
          std::string ioOp;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Type) {
              templateType = arg.getAsType().getCanonicalType();
              if (defaultEndian)
                ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExpr);
              else
                ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExpr, endianExprStr);
            }
          }

          if (ioOp.empty()) {
            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocation(), AthenaError);
            diag.AddString("Unable to use type '"s.append(templateType.getAsString()).append("' with Athena"));
            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
            continue;
          }

          if (sizeExpr.empty()) {
            clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocation(), AthenaError);
            diag.AddString("Unable to use count variable with Athena");
            diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
            continue;
          }

          fileOut << "  AT_PROP_CASE(" << propIdExpr << "):\n"
                  << "    Do" << ioOp << ";\n"
                  << "    return true;\n";
        } else if (tsDecl->getName() == "Buffer") {
          const clang::Expr* sizeExpr = nullptr;
          std::string sizeExprStr;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const auto* uExpr =
                  static_cast<const clang::UnaryExprOrTypeTraitExpr*>(arg.getAsExpr()->IgnoreImpCasts());
              if (uExpr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                  uExpr->getKind() == clang::UETT_SizeOf) {
                const clang::Expr* argExpr = uExpr->getArgumentExpr();
                while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                  argExpr = static_cast<const clang::ParenExpr*>(argExpr)->getSubExpr();
                sizeExpr = argExpr;
                llvm::raw_string_ostream strStream(sizeExprStr);
                argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
              }
            }
          }
          if (sizeExprStr.empty()) {
            if (sizeExpr) {
              clang::DiagnosticBuilder diag = context.getDiagnostics().Report(sizeExpr->getExprLoc(), AthenaError);
              diag.AddString("Unable to use size variable with Athena");
              diag.AddSourceRange(clang::CharSourceRange(sizeExpr->getSourceRange(), true));
            } else {
              clang::DiagnosticBuilder diag = context.getDiagnostics().Report(field->getLocation(), AthenaError);
              diag.AddString("Unable to use size variable with Athena");
              diag.AddSourceRange(clang::CharSourceRange(field->getSourceRange(), true));
            }
            continue;
          }

          std::string ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr);

          fileOut << "  AT_PROP_CASE(" << propIdExpr << "):\n"
                  << "    Do" << ioOp << ";\n"
                  << "    return true;\n";
        } else if (tsDecl->getName() == "String") {
          std::string sizeExprStr;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
              const auto* uExpr = static_cast<const clang::UnaryExprOrTypeTraitExpr*>(expr);
              llvm::APSInt sizeLiteral;
              if (expr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                  uExpr->getKind() == clang::UETT_SizeOf) {
                const clang::Expr* argExpr = uExpr->getArgumentExpr();
                while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                  argExpr = static_cast<const clang::ParenExpr*>(argExpr)->getSubExpr();
                llvm::raw_string_ostream strStream(sizeExprStr);
                argExpr->printPretty(strStream, nullptr, context.getPrintingPolicy());
              } else if (GetIntegerConstantExpr(expr, sizeLiteral, context)) {
#if LLVM_VERSION_MAJOR >= 13
                llvm::SmallVector<char> strStream;
                sizeLiteral.toString(strStream, 10);
                sizeExprStr = std::string(strStream.begin(), strStream.end());
#else
                sizeExprStr = sizeLiteral.toString(10);
#endif
              }
            }
          }

          std::string ioOp;
          if (!sizeExprStr.empty())
            ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr);
          else
            ioOp = GetOpString(fieldName, propIdExpr);

          fileOut << "  AT_PROP_CASE(" << propIdExpr << "):\n"
                  << "    Do" << ioOp << ";\n"
                  << "    return true;\n";
        } else if (tsDecl->getName() == "WString") {
          llvm::APSInt endian(64, -1);
          std::string endianExprStr;
          bool defaultEndian = true;
          if (classParms->size() >= 2) {
            const clang::NamedDecl* endianParm = classParms->getParam(1);
            if (endianParm->getKind() == clang::Decl::NonTypeTemplateParm) {
              const auto* nttParm = static_cast<const clang::NonTypeTemplateParmDecl*>(endianParm);
              llvm::raw_string_ostream strStream(endianExprStr);
              nttParm->print(strStream, context.getPrintingPolicy());
            }
          }

          std::string sizeExprStr;
          size_t idx = 0;
          for (const clang::TemplateArgument& arg : *tsType) {
            if (arg.getKind() == clang::TemplateArgument::Expression) {
              const clang::Expr* expr = arg.getAsExpr()->IgnoreImpCasts();
              if (idx == 0) {
                llvm::APSInt sizeLiteral;
                const auto* uExpr = static_cast<const clang::UnaryExprOrTypeTraitExpr*>(expr);
                if (expr->getStmtClass() == clang::Stmt::UnaryExprOrTypeTraitExprClass &&
                    uExpr->getKind() == clang::UETT_SizeOf) {
                  const clang::Expr* argExpr = uExpr->getArgumentExpr();
                  while (argExpr->getStmtClass() == clang::Stmt::ParenExprClass)
                    argExpr = static_cast<const clang::ParenExpr*>(argExpr)->getSubExpr();
                  llvm::raw_string_ostream strStream2(sizeExprStr);
                  argExpr->printPretty(strStream2, nullptr, context.getPrintingPolicy());
                } else if (GetIntegerConstantExpr(expr, sizeLiteral, context)) {
#if LLVM_VERSION_MAJOR >= 13
                  llvm::SmallVector<char> strStream;
                  sizeLiteral.toString(strStream, 10);
                  sizeExprStr = std::string(strStream.begin(), strStream.end());
#else
                  sizeExprStr = sizeLiteral.toString(10);
#endif
                }
              } else if (idx == 1) {
                llvm::raw_string_ostream strStream(endianExprStr);
                expr->printPretty(strStream, nullptr, context.getPrintingPolicy());
                defaultEndian = false;
              }
            }
            ++idx;
          }

          std::string ioOp;
          if (!sizeExprStr.empty()) {
            if (defaultEndian)
              ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr);
            else
              ioOp = GetVectorOpString(fieldName, propIdExpr, sizeExprStr, endianExprStr);
          } else {
            if (defaultEndian)
              ioOp = GetOpString(fieldName, propIdExpr);
            else
              ioOp = GetOpString(fieldName, propIdExpr, endianExprStr);
          }

          fileOut << "  AT_PROP_CASE(" << propIdExpr << "):\n"
                  << "    Do" << ioOp << ";\n"
                  << "    return true;\n";
        } else {
          const clang::NamedDecl* nd = tsDecl->getTemplatedDecl();
          if (const clang::CXXRecordDecl* rd = clang::dyn_cast_or_null<clang::CXXRecordDecl>(nd)) {
            std::string baseDNA2;
            if (isDNARecord(rd, baseDNA2)) {
              fileOut << "  AT_PROP_CASE(" << propIdExpr << "):\n"
                      << "    Do" << GetOpString(fieldName, propIdExpr) << ";\n"
                      << "    return true;\n";
            }
          }
        }
      } else if (regType->getTypeClass() == clang::Type::Record) {
        const clang::CXXRecordDecl* cxxRDecl = regType->getAsCXXRecordDecl();
        std::string baseDNA2;
        if (cxxRDecl && isDNARecord(cxxRDecl, baseDNA2)) {
          fileOut << "  AT_PROP_CASE(" << propIdExpr << "):\n"
                  << "    Do" << GetOpString(fieldName, propIdExpr) << ";\n"
                  << "    return true;\n";
        }
      }
    }

    fileOut << "  default:\n    return false;\n  }\n}\n\n";
  }

public:
  explicit ATDNAEmitVisitor(clang::ASTContext& ctxin, StreamOut& fo) : context(ctxin), fileOut(fo) {}

  bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl) {
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

    /* Determine if is is a YAML DNA */
    bool isYamlDNA = false;
    for (const clang::CXXMethodDecl* method : decl->methods())
      if (method->getDeclName().isIdentifier() && (method->getName() == "read" || method->getName() == "write") &&
          method->getNumParams() == 1 &&
          method->getParamDecl(0)->getType().getAsString() == "athena::io::YAMLDocReader &") {
        isYamlDNA = true;
        break;
      }

    /* Determine if this is a regular DNA or PropDNA */
    bool isPropDNA = false;
    for (const clang::Decl* d : decl->decls())
      if (const clang::FunctionTemplateDecl* m = clang::dyn_cast_or_null<clang::FunctionTemplateDecl>(d))
        if (m->getDeclName().isIdentifier() && m->getName() == "Lookup") {
          isPropDNA = true;
          break;
        }

    /* Make sure there isn't Delete meta type */
    for (const clang::FieldDecl* field : decl->fields()) {
      clang::QualType qualType = field->getType().getCanonicalType();
      const clang::Type* regType = qualType.getTypePtrOrNull();
      if (regType) {
        const clang::CXXRecordDecl* rDecl = regType->getAsCXXRecordDecl();
        if (rDecl) {
          if (rDecl->getName() == "Delete") {
            const clang::CXXRecordDecl* rParentDecl = llvm::dyn_cast_or_null<clang::CXXRecordDecl>(rDecl->getParent());
            if (rParentDecl) {
              std::string parentCheck = rParentDecl->getTypeForDecl()->getCanonicalTypeInternal().getAsString();
              if (!parentCheck.compare(0, sizeof(ATHENA_DNA_BASETYPE) - 1, ATHENA_DNA_BASETYPE))
                return true;
            }
          }
        }
      }
    }

    std::vector<std::pair<std::string, int>> specializations = GetNestedTypeSpecializations(decl);

    emitEnumerateFunc(decl, baseDNA);
    if (isPropDNA) {
      emitLookupFunc(decl, baseDNA);
      for (const auto& specialization : specializations)
        fileOut << "AT_SPECIALIZE_PROPDNA(" << specialization.first << ")\n";
    } else if (isYamlDNA) {
      for (const auto& specialization : specializations)
        fileOut << "AT_SPECIALIZE_DNA_YAML(" << specialization.first << ")\n";
    } else {
      for (const auto& specialization : specializations)
        fileOut << "AT_SPECIALIZE_DNA(" << specialization.first << ")\n";
    }
    fileOut << "\n\n";

    for (const auto& specialization : specializations) {
      for (int i = 0; i < specialization.second; ++i)
        fileOut << "template <>\n";
      fileOut << "std::string_view " << specialization.first << "::DNAType() {\n  return \"" << specialization.first
              << "\"sv;\n}\n";
    }
    fileOut << "\n\n";

    return true;
  }
};

class ATDNAConsumer : public clang::ASTConsumer {
  std::unique_ptr<StreamOut> fileOut;
  StreamOut& fileOutOld;
  ATDNAEmitVisitor emitVisitor;

public:
  explicit ATDNAConsumer(clang::ASTContext& context, std::unique_ptr<StreamOut>&& fo, StreamOut* foOld)
  : fileOut(std::move(fo)), fileOutOld(*foOld), emitVisitor(context, *foOld) {}

  void HandleTranslationUnit(clang::ASTContext& context) override {
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

class ATDNAAction : public clang::ASTFrontendAction {
  /* Used by LLVM 3.9+; client owns stream */
  std::unique_ptr<StreamOut> MakeStreamOut(std::unique_ptr<StreamOut>&& so, StreamOut*& outPtr) {
    outPtr = so.get();
    return std::move(so);
  }

  /* Used by previous versions of LLVM; CompilerInstance owns stream */
  std::unique_ptr<StreamOut> MakeStreamOut(StreamOut* so, StreamOut*& outPtr) {
    outPtr = so;
    return {};
  }

#if LLVM_VERSION_MAJOR >= 9
  llvm::Optional<clang::DependencyFileGenerator> TheDependencyFileGenerator;
#else
  std::unique_ptr<clang::DependencyFileGenerator> TheDependencyFileGenerator;
#endif

public:
  explicit ATDNAAction() = default;
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance& compiler,
                                                        llvm::StringRef /*filename*/) override {
    clang::DependencyOutputOptions DepOpts;
    DepOpts.OutputFile = DepFileOut.getValue();
    DepOpts.Targets = DepFileTargets;
#if LLVM_VERSION_MAJOR >= 9
    if (!DepOpts.OutputFile.empty()) {
      TheDependencyFileGenerator.emplace(DepOpts);
      TheDependencyFileGenerator->attachToPreprocessor(compiler.getPreprocessor());
    }
#else
    if (!DepOpts.OutputFile.empty())
      TheDependencyFileGenerator.reset(
          clang::DependencyFileGenerator::CreateAndAttachToPreprocessor(compiler.getPreprocessor(), DepOpts));
#endif

    std::unique_ptr<StreamOut> fileout;
    StreamOut* fileoutOld;
    if (OutputFilename.size())
#if LLVM_VERSION_MAJOR >= 12
      fileout = MakeStreamOut(compiler.createOutputFile(OutputFilename, false, true, true), fileoutOld);
#else
      fileout = MakeStreamOut(compiler.createOutputFile(OutputFilename, false, true, "", "", true), fileoutOld);
#endif
    else
      fileout = MakeStreamOut(compiler.createDefaultOutputFile(false, "a", "cpp"), fileoutOld);
    AthenaError =
        compiler.getASTContext().getDiagnostics().getCustomDiagID(clang::DiagnosticsEngine::Error, "Athena error: %0");
    return std::unique_ptr<clang::ASTConsumer>(
        new ATDNAConsumer(compiler.getASTContext(), std::move(fileout), fileoutOld));
  }
};

int main(int argc, const char** argv) {
  llvm::cl::ParseCommandLineOptions(argc, argv, "Athena DNA Generator");
  if (Help)
    llvm::cl::PrintHelpMessage();

  std::vector<std::string> args = {
    XSTR(ATDNA_ARGV0),
    "-fsyntax-only",
    "-std=c++2a",
#if __x86_64__
    "-mno-sse",
#endif
    "-D__atdna__=1",
    "-Wno-expansion-to-defined",
    "-Wno-nullability-completeness",
    "-Werror=shadow-field",
    "-I" XSTR(INSTALL_PREFIX) "/include/Athena",
  };
  for (int a = 1; a < argc; ++a) {
    args.emplace_back(argv[a]);
  }

  llvm::IntrusiveRefCntPtr<clang::FileManager> fman(new clang::FileManager(clang::FileSystemOptions()));
#if LLVM_VERSION_MAJOR >= 10
  clang::tooling::ToolInvocation TI(std::move(args), std::make_unique<ATDNAAction>(), fman.get());
#else
  ATDNAAction* action = new ATDNAAction();
  clang::tooling::ToolInvocation TI(std::move(args), action, fman.get());
#endif
  if (!TI.run())
    return 1;
  return 0;
}
