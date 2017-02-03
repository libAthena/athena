#ifndef DNAYAML_HPP
#define DNAYAML_HPP

/* BIG FAT WARNING!!!
 *
 * The type-structure of this file is expected to remain consistent for 'atdna'
 * Any changes to the types or namespacing must be reflected in 'atdna/main.cpp'
 */

#include <string.h>
#include <yaml.h>
#include <utf8proc.h>
#include "DNA.hpp"
#include "FileReader.hpp"

namespace athena
{
namespace io
{

struct YAMLNode
{
    yaml_node_type_t m_type;
    std::string m_scalarString;
    std::vector<std::unique_ptr<YAMLNode>> m_seqChildren;
    std::vector<std::pair<std::string, std::unique_ptr<YAMLNode>>> m_mapChildren;
    YAMLNode(yaml_node_type_t type) : m_type(type) {}
    inline const YAMLNode* findMapChild(const char* key) const
    {
        for (const auto& item : m_mapChildren)
            if (!item.first.compare(key))
                return item.second.get();
        return nullptr;
    }
};

template <typename RETURNTYPE>
RETURNTYPE NodeToVal(const YAMLNode* node);

template <>
bool NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(bool val);

template <>
atInt8 NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(atInt8 val);

template <>
atUint8 NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(atUint8 val);

template <>
atInt16 NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(atInt16 val);

template <>
atUint16 NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(atUint16 val);

template <>
atInt32 NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(atInt32 val);

template <>
atUint32 NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(atUint32 val);

template <>
atInt64 NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(atInt64 val);

template <>
atUint64 NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(atUint64 val);

template <>
float NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(float val);

template <>
double NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(double val);

template <typename RETURNTYPE>
RETURNTYPE NodeToVec(const YAMLNode* node);

template <>
atVec2f NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(const atVec2f& val);

template <>
atVec3f NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(const atVec3f& val);

template <>
atVec4f NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(const atVec4f& val);

template <>
atVec2d NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(const atVec2d& val);

template <>
atVec3d NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(const atVec3d& val);

template <>
atVec4d NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(const atVec4d& val);

template <>
std::unique_ptr<atUint8[]> NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(const std::unique_ptr<atUint8[]>& val, size_t byteCount);

template <>
std::string NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(const std::string& val);

std::unique_ptr<YAMLNode> ValToNode(const char* val);

template <>
std::wstring NodeToVal(const YAMLNode* node);

std::unique_ptr<YAMLNode> ValToNode(const std::wstring& val);

std::unique_ptr<YAMLNode> ValToNode(const wchar_t* val);

std::unique_ptr<YAMLNode> ValToNode(const std::u16string& val);

std::unique_ptr<YAMLNode> ValToNode(const char16_t* val);

std::unique_ptr<YAMLNode> ValToNode(const std::u32string& val);

std::unique_ptr<YAMLNode> ValToNode(const char32_t* val);

std::string base64_encode(const atUint8* bytes_to_encode, size_t in_len);
std::unique_ptr<atUint8[]> base64_decode(const std::string& encoded_string);

void HandleYAMLParserError(yaml_parser_t* parser);
void HandleYAMLEmitterError(yaml_emitter_t* emitter);

struct YAMLStdStringReaderState
{
    std::string::const_iterator begin;
    std::string::const_iterator end;
    YAMLStdStringReaderState(const std::string& str)
    {
        begin = str.begin();
        end = str.end();
    }
};
int YAMLStdStringReader(YAMLStdStringReaderState* str,
                        unsigned char* buffer, size_t size, size_t* size_read);
int YAMLStdStringWriter(std::string* str, unsigned char* buffer, size_t size);

class YAMLDocReader
{
    std::unique_ptr<YAMLNode> m_rootNode;
    std::vector<YAMLNode*> m_subStack;
    std::vector<int> m_seqTrackerStack;
    yaml_parser_t m_parser;
    std::unique_ptr<YAMLNode> ParseEvents(athena::io::IStreamReader* reader);

public:
    YAMLDocReader();
    ~YAMLDocReader();

    void reset();

    inline yaml_parser_t* getParser() { return &m_parser; }

    bool parse(athena::io::IStreamReader* reader);

    bool ClassTypeOperation(std::function<bool(const char* dnaType)> func);
    bool ValidateClassType(const char* expectedType);

    inline const YAMLNode* getRootNode() const { return m_rootNode.get(); }
    inline const YAMLNode* getCurNode() const { return m_subStack.empty() ? nullptr : m_subStack.back(); }
    std::unique_ptr<YAMLNode> releaseRootNode() { return std::move(m_rootNode); }

    bool enterSubRecord(const char* name);
    void leaveSubRecord();

    template <class T>
    void enumerate(const char* name, T& record)
    {
        enterSubRecord(name);
        record.read(*this);
        leaveSubRecord();
    }

    bool enterSubVector(const char* name, size_t& countOut);
    void leaveSubVector();

    template <class T>
    size_t enumerate(const char* name, std::vector<T>& vector,
                     typename std::enable_if<!std::is_arithmetic<T>::value &&
                                             !std::is_same<T, atVec2f>::value &&
                                             !std::is_same<T, atVec3f>::value &&
                                             !std::is_same<T, atVec4f>::value>::type* = 0)
    {
        size_t countOut;
        enterSubVector(name, countOut);
        vector.clear();
        vector.reserve(countOut);
        for (size_t i=0 ; i<countOut ; ++i)
        {
            vector.emplace_back();
            enterSubRecord(nullptr);
            vector.back().read(*this);
            leaveSubRecord();
        }
        leaveSubVector();
        return countOut;
    }

    template <class T>
    size_t enumerate(const char* name, std::vector<T>& vector,
                     typename std::enable_if<std::is_arithmetic<T>::value ||
                                             std::is_same<T, atVec2f>::value ||
                                             std::is_same<T, atVec3f>::value ||
                                             std::is_same<T, atVec4f>::value>::type* = 0)
    {
        size_t countOut;
        enterSubVector(name, countOut);
        vector.clear();
        vector.reserve(countOut);
        for (size_t i=0 ; i<countOut ; ++i)
            vector.push_back(readVal<T>(name));
        leaveSubVector();
        return countOut;
    }

    template <class T>
    size_t enumerate(const char* name, std::vector<T>& vector,
                     std::function<void(YAMLDocReader&, T&)> readf)
    {
        size_t countOut;
        enterSubVector(name, countOut);
        vector.clear();
        vector.reserve(countOut);
        for (size_t i=0 ; i<countOut ; ++i)
        {
            vector.emplace_back();
            enterSubRecord(nullptr);
            readf(*this, vector.back());
            leaveSubRecord();
        }
        leaveSubVector();
        return countOut;
    }

    template <typename RETURNTYPE>
    RETURNTYPE readVal(const char* name);
    bool readBool(const char* name);
    atInt8 readByte(const char* name);
    atUint8 readUByte(const char* name);
    atInt16 readInt16(const char* name);
    atUint16 readUint16(const char* name);
    atInt32 readInt32(const char* name);
    atUint32 readUint32(const char* name);
    atInt64 readInt64(const char* name);
    atUint64 readUint64(const char* name);
    float readFloat(const char* name);
    double readDouble(const char* name);
    atVec2f readVec2f(const char* name);
    atVec3f readVec3f(const char* name);
    atVec4f readVec4f(const char* name);
    atVec2d readVec2d(const char* name);
    atVec3d readVec3d(const char* name);
    atVec4d readVec4d(const char* name);
    std::unique_ptr<atUint8[]> readUBytes(const char* name);
    std::string readString(const char* name);
    std::wstring readWString(const char* name);
};

class YAMLDocWriter
{
    YAMLNode m_rootNode;
    std::vector<YAMLNode*> m_subStack;
    yaml_emitter_t m_emitter;
    static bool RecursiveFinish(yaml_emitter_t* doc, const YAMLNode& node);
public:
    YAMLDocWriter(const char* classType);
    ~YAMLDocWriter();

    yaml_emitter_t* getEmitter() { return &m_emitter; }

    bool finish(athena::io::IStreamWriter* fout);

    inline YAMLNode* getCurNode() const { return m_subStack.empty() ? nullptr : m_subStack.back(); }

    void enterSubRecord(const char* name);
    void leaveSubRecord();

    template <class T>
    void enumerate(const char* name, T& record)
    {
        enterSubRecord(name);
        record.write(*this);
        leaveSubRecord();
    }

    void enterSubVector(const char* name);
    void leaveSubVector();

    template <class T>
    void enumerate(const char* name, const std::vector<T>& vector,
                   typename std::enable_if<!std::is_arithmetic<T>::value &&
                                           !std::is_same<T, atVec2f>::value &&
                                           !std::is_same<T, atVec3f>::value &&
                                           !std::is_same<T, atVec4f>::value &&
                                           !std::is_same<T, atVec2d>::value &&
                                           !std::is_same<T, atVec3d>::value &&
                                            !std::is_same<T, atVec4d>::value>::type* = 0)
    {
        enterSubVector(name);
        for (const T& item : vector)
        {
            enterSubRecord(nullptr);
            item.write(*this);
            leaveSubRecord();
        }
        leaveSubVector();
    }

    template <class T>
    void enumerate(const char* name, const std::vector<T>& vector,
                   typename std::enable_if<std::is_arithmetic<T>::value ||
                                           std::is_same<T, atVec2f>::value ||
                                           std::is_same<T, atVec3f>::value ||
                                           std::is_same<T, atVec4f>::value ||
                                           std::is_same<T, atVec2d>::value ||
                                           std::is_same<T, atVec3d>::value ||
                                           std::is_same<T, atVec4d>::value>::type* = 0)
    {
        enterSubVector(name);
        for (T item : vector)
            writeVal<T>(nullptr, item);
        leaveSubVector();
    }

    template <class T>
    void enumerate(const char* name, const std::vector<T>& vector,
                   std::function<void(YAMLDocWriter&, const T&)> writef)
    {
        enterSubVector(name);
        for (const T& item : vector)
        {
            enterSubRecord(nullptr);
            writef(*this, item);
            leaveSubRecord();
        }
        leaveSubVector();
    }

    template <typename INTYPE>
    void writeVal(const char* name, const INTYPE& val);
    template <typename INTYPE>
    void writeVal(const char* name, const INTYPE& val, size_t byteCount);
    void writeBool(const char* name, const bool& val);
    void writeByte(const char* name, const atInt8& val);
    void writeUByte(const char* name, const atUint8& val);
    void writeInt16(const char* name, const atInt16& val);
    void writeUint16(const char* name, const atUint16& val);
    void writeInt32(const char* name, const atInt32& val);
    void writeUint32(const char* name, const atUint32& val);
    void writeInt64(const char* name, const atInt64& val);
    void writeUint64(const char* name, const atUint64& val);
    void writeFloat(const char* name, const float& val);
    void writeDouble(const char* name, const double& val);
    void writeVec2f(const char* name, const atVec2f& val);
    void writeVec3f(const char* name, const atVec3f& val);
    void writeVec4f(const char* name, const atVec4f& val);
    void writeVec2d(const char* name, const atVec2d& val);
    void writeVec3d(const char* name, const atVec3d& val);
    void writeVec4d(const char* name, const atVec4d& val);
    void writeUBytes(const char* name, const std::unique_ptr<atUint8[]>& val, size_t byteCount);
    void writeString(const char* name, const std::string& val);
    void writeString(const char* name, const char* val);
    void writeWString(const char* name, const std::wstring& val);
    void writeWString(const char* name, const wchar_t* val);
    void writeU16String(const char* name, const std::u16string& val);
    void writeU16String(const char* name, const char16_t* val);
    void writeU32String(const char* name, const std::u32string& val);
    void writeU32String(const char* name, const char32_t* val);
};

int YAMLAthenaReader(athena::io::IStreamReader* reader,
                     unsigned char* buffer, size_t size, size_t* size_read);

int YAMLAthenaWriter(athena::io::IStreamWriter* writer,
                     unsigned char *buffer, size_t size);

/* forward-declaration dance for recursively-derived types */

template <size_t sizeVar, Endian VE>
struct BufferYaml;

template <atInt32 sizeVar, Endian VE>
struct StringYaml;

template <atInt32 sizeVar, Endian VE>
struct WStringYaml;

template <atInt32 sizeVar, Endian VE>
struct WStringAsStringYaml;

template <Endian DNAE>
struct DNAYaml : DNA<DNAE>
{
    virtual ~DNAYaml() = default;

    virtual void read(IStreamReader& r)=0;
    virtual void write(IStreamWriter& w) const=0;
    virtual void read(YAMLDocReader& in)=0;
    virtual void write(YAMLDocWriter& out) const=0;
    static const char* DNAType() { return nullptr; }
    virtual const char* DNATypeV() const { return nullptr; }

    template <size_t sizeVar>
    using Buffer = struct athena::io::BufferYaml<sizeVar, DNAE>;

    template <atInt32 sizeVar = -1>
    using String = struct athena::io::StringYaml<sizeVar, DNAE>;

    template <atInt32 sizeVar = -1, Endian VE = DNAE>
    using WString = struct athena::io::WStringYaml<sizeVar, VE>;

    template <atInt32 sizeVar = -1>
    using WStringAsString = struct athena::io::WStringAsStringYaml<sizeVar, DNAE>;

    std::string toYAMLString() const
    {
        YAMLDocWriter docWriter(DNATypeV());

        std::string res;
        yaml_emitter_set_output(docWriter.getEmitter(), (yaml_write_handler_t*)YAMLStdStringWriter, &res);
        yaml_emitter_set_unicode(docWriter.getEmitter(), true);
        yaml_emitter_set_width(docWriter.getEmitter(), -1);

        write(docWriter);
        if (!docWriter.finish(nullptr))
            return std::string();

        return res;
    }

    bool fromYAMLString(const std::string& str)
    {
        YAMLStdStringReaderState reader(str);
        YAMLDocReader docReader;
        yaml_parser_set_input(docReader.getParser(), (yaml_read_handler_t*)YAMLStdStringReader, &reader);
        if (!docReader.parse(nullptr))
            return false;
        read(docReader);
        return true;
    }

    template<class DNASubtype>
    static bool ValidateFromYAMLString(const std::string& str)
    {
        YAMLStdStringReaderState reader(str);
        YAMLDocReader docReader;
        yaml_parser_set_input(docReader.getParser(), (yaml_read_handler_t*)YAMLStdStringReader, &reader);
        bool retval = docReader.ValidateClassType(DNASubtype::DNAType());
        return retval;
    }

    bool toYAMLStream(athena::io::IStreamWriter& fout) const
    {
        YAMLDocWriter docWriter(DNATypeV());

        yaml_emitter_set_unicode(docWriter.getEmitter(), true);
        yaml_emitter_set_width(docWriter.getEmitter(), -1);

        write(docWriter);
        if (!docWriter.finish(&fout))
            return false;

        return true;
    }

    typedef void (DNAYaml::*YAMLWriteMemberFn)(YAMLDocWriter& out) const;
    bool toYAMLStream(athena::io::IStreamWriter& fout, YAMLWriteMemberFn fn) const
    {
        YAMLDocWriter docWriter(DNATypeV());

        yaml_emitter_set_unicode(docWriter.getEmitter(), true);
        yaml_emitter_set_width(docWriter.getEmitter(), -1);

        (this->*fn)(docWriter);
        if (!docWriter.finish(&fout))
            return false;

        return true;
    }

    bool fromYAMLStream(athena::io::IStreamReader& fin)
    {
        YAMLDocReader docReader;
        if (!docReader.parse(&fin))
            return false;
        read(docReader);
        return true;
    }

    typedef void (DNAYaml::*YAMLReadMemberFn)(YAMLDocReader& in);
    bool fromYAMLStream(athena::io::IStreamReader& fin, YAMLReadMemberFn fn)
    {
        YAMLDocReader docReader;
        if (!docReader.parse(&fin))
            return false;
        (this->*fn)(docReader);
        return true;
    }

    template<class DNASubtype>
    static bool ValidateFromYAMLStream(athena::io::IStreamReader& fin)
    {
        YAMLDocReader reader;
        atUint64 pos = fin.position();
        yaml_parser_set_input(reader.getParser(), (yaml_read_handler_t*)YAMLAthenaReader, &fin);
        bool retval = reader.ValidateClassType(DNASubtype::DNAType());
        fin.seek(pos, athena::Begin);
        return retval;
    }
};

template <size_t sizeVar, Endian VE>
struct BufferYaml : public DNAYaml<VE>, public std::unique_ptr<atUint8[]>
{
    typename DNA<VE>::Delete expl;
    void read(IStreamReader& reader)
    {
        reset(new atUint8[sizeVar]);
        reader.readUBytesToBuf(get(), sizeVar);
    }
    void write(IStreamWriter& writer) const
    {
        writer.writeUBytes(get(), sizeVar);
    }
    size_t binarySize(size_t __isz) const
    {
        return __isz + sizeVar;
    }
    void read(athena::io::YAMLDocReader& reader)
    {*this = reader.readUBytes(nullptr);}
    void write(athena::io::YAMLDocWriter& writer) const
    {writer.writeUBytes(nullptr, *this, sizeVar);}
};

template <atInt32 sizeVar, Endian VE>
struct StringYaml : public DNAYaml<VE>, public std::string
{
    typename DNA<VE>::Delete expl;
    void read(IStreamReader& reader)
    {this->assign(std::move(reader.readString(sizeVar)));}
    void write(IStreamWriter& writer) const
    {writer.writeString(*this, sizeVar);}
    size_t binarySize(size_t __isz) const
    {return __isz + ((sizeVar<0)?(this->size()+1):sizeVar);}
    void read(athena::io::YAMLDocReader& reader)
    {this->assign(std::move(reader.readString(nullptr)));}
    void write(athena::io::YAMLDocWriter& writer) const
    {writer.writeString(nullptr, *this);}
    StringYaml() = default;
    StringYaml(const std::string& __str) : std::string(__str) {}
    StringYaml(std::string&& __str) : std::string(std::move(__str)) {}
    std::string& operator=(const std::string& __str)
    {return this->assign(__str);}
    std::string& operator=(std::string&& __str)
    {static_cast<std::string&>(*this) = std::move(__str); return *this;}
};

template <atInt32 sizeVar, Endian VE>
struct WStringYaml : public DNAYaml<VE>, public std::wstring
{
    typename DNA<VE>::Delete expl;
    void read(IStreamReader& reader)
    {
        reader.setEndian(VE);
        this->assign(std::move(reader.readWString(sizeVar)));
    }
    void write(IStreamWriter& writer) const
    {
        writer.setEndian(VE);
        writer.writeWString(*this, sizeVar);
    }
    size_t binarySize(size_t __isz) const
    {return __isz + (((sizeVar<0)?(this->size()+1):sizeVar)*2);}
    void read(athena::io::YAMLDocReader& reader)
    {this->assign(std::move(reader.readWString(nullptr)));}
    void write(athena::io::YAMLDocWriter& writer) const
    {writer.writeWString(nullptr, *this);}
    std::wstring& operator=(const std::wstring& __str)
    {return this->assign(__str);}
    std::wstring& operator=(std::wstring&& __str)
    {this->swap(__str); return *this;}
};

template <atInt32 sizeVar, Endian VE>
struct WStringAsStringYaml : public DNAYaml<VE>, public std::string
{
    typename DNA<VE>::Delete expl;
    void read(IStreamReader& reader)
    {*this = reader.readWStringAsString(sizeVar);}
    void write(IStreamWriter& writer) const
    {writer.writeStringAsWString(*this, sizeVar);}
    size_t binarySize(size_t __isz) const
    {return __isz + (((sizeVar<0)?(this->size()+1):sizeVar)*2);}
    void read(athena::io::YAMLDocReader& reader)
    {this->assign(std::move(reader.readString(nullptr)));}
    void write(athena::io::YAMLDocWriter& writer) const
    {writer.writeString(nullptr, *this);}
    std::string& operator=(const std::string& __str)
    {return this->assign(__str);}
    std::string& operator=(std::string&& __str)
    {this->swap(__str); return *this;}
};

/** Macro to automatically declare YAML read/write methods in subclasses */
#define DECL_YAML \
    DECL_DNA \
    void read(athena::io::YAMLDocReader&); \
    void write(athena::io::YAMLDocWriter&) const; \
    static const char* DNAType(); \
    const char* DNATypeV() const {return DNAType();} \

/** Macro to automatically declare YAML read/write methods with client-code's definition */
#define DECL_EXPLICIT_YAML \
    void read(athena::io::YAMLDocReader&); \
    void write(athena::io::YAMLDocWriter&) const; \
    static const char* DNAType(); \
    const char* DNATypeV() const {return DNAType();} \

}
}

#endif // DNAYAML_HPP
