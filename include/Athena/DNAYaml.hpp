#ifndef DNAYAML_HPP
#define DNAYAML_HPP

/* BIG FAT WARNING!!!
 *
 * The type-structure of this file is expected to remain consistent for 'atdna'
 * Any changes to the types or namespacing must be reflected in 'atdna/main.cpp'
 */

#include <string.h>
#include <yaml.h>
#include "DNA.hpp"

namespace Athena
{
namespace io
{

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

template <typename INTYPE>
std::unique_ptr<YAMLNode> ValToNode(const INTYPE& val);
template <typename INTYPE>
std::unique_ptr<YAMLNode> ValToNode(const INTYPE* val);
template <typename INTYPE>
std::unique_ptr<YAMLNode> ValToNode(const INTYPE& val, size_t byteCount);

template <>
inline bool NodeToVal(const YAMLNode* node)
{
    char firstCh = tolower(node->m_scalarString[0]);
    if (firstCh == 't')
        return true;
    else if (firstCh == 'f')
        return false;
    else if (isdigit(firstCh) && firstCh != 0)
        return true;
    return false;
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const bool& val)
{
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = val?"True":"False";
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline atInt8 NodeToVal(const YAMLNode* node)
{
    return strtol(node->m_scalarString.c_str(), NULL, 0);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atInt8& val)
{
    char str[32];
    snprintf(str, 32, "0x%02X", val);
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = str;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline atUint8 NodeToVal(const YAMLNode* node)
{
    return strtoul(node->m_scalarString.c_str(), NULL, 0);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atUint8& val)
{
    char str[32];
    snprintf(str, 32, "0x%02X", val);
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = str;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline atInt16 NodeToVal(const YAMLNode* node)
{
    return strtol(node->m_scalarString.c_str(), NULL, 0);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atInt16& val)
{
    char str[32];
    snprintf(str, 32, "0x%04X", val);
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = str;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline atUint16 NodeToVal(const YAMLNode* node)
{
    return strtoul(node->m_scalarString.c_str(), NULL, 0);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atUint16& val)
{
    char str[32];
    snprintf(str, 32, "0x%04X", val);
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = str;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline atInt32 NodeToVal(const YAMLNode* node)
{
    return strtol(node->m_scalarString.c_str(), NULL, 0);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atInt32& val)
{
    char str[32];
    snprintf(str, 32, "0x%08X", val);
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = str;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline atUint32 NodeToVal(const YAMLNode* node)
{
    return strtoul(node->m_scalarString.c_str(), NULL, 0);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atUint32& val)
{
    char str[32];
    snprintf(str, 32, "0x%08X", val);
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = str;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline atInt64 NodeToVal(const YAMLNode* node)
{
#if _WIN32
    return _strtoi64(node->m_scalarString.c_str(), NULL, 0);
#else
    return strtoq(node->m_scalarString.c_str(), NULL, 0);
#endif
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atInt64& val)
{
    char str[32];
    snprintf(str, 32, "0x%016llX", val);
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = str;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline atUint64 NodeToVal(const YAMLNode* node)
{
#if _WIN32
    return _strtoui64(node->m_scalarString.c_str(), NULL, 0);
#else
    return strtouq(node->m_scalarString.c_str(), NULL, 0);
#endif
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atUint64& val)
{
    char str[32];
    snprintf(str, 32, "0x%016llX", val);
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = str;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline float NodeToVal(const YAMLNode* node)
{
    return strtof(node->m_scalarString.c_str(), NULL);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const float& val)
{
    char str[64];
    snprintf(str, 64, "%f", val);
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = str;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline double NodeToVal(const YAMLNode* node)
{
    return strtod(node->m_scalarString.c_str(), NULL);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const double& val)
{
    char str[64];
    snprintf(str, 64, "%f", val);
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = str;
    return std::unique_ptr<YAMLNode>(ret);
}

template <typename RETURNTYPE>
inline RETURNTYPE NodeToVec(const YAMLNode* node)
{
    RETURNTYPE retval = {};
    auto it = node->m_seqChildren.begin();
    for (size_t i=0;
         i<4 && it != node->m_seqChildren.end();
         ++i, ++it)
    {
        YAMLNode* snode = it->get();
        if (snode->m_type == YAML_SCALAR_NODE)
            retval.vec[i] = NodeToVal<float>(snode);
        else
            retval.vec[i] = 0.0;
    }
    return retval;
}

template <>
inline atVec2f NodeToVal(const YAMLNode* node)
{
    return NodeToVec<atVec2f>(node);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atVec2f& val)
{
    YAMLNode* ret = new YAMLNode(YAML_SEQUENCE_NODE);
    ret->m_seqChildren.reserve(2);
    for (size_t i=0 ; i<2 ; ++i)
    {
        char str[64];
        snprintf(str, 64, "%f", val.vec[i]);
        YAMLNode* comp = new YAMLNode(YAML_SCALAR_NODE);
        comp->m_scalarString = str;
        ret->m_seqChildren.emplace_back(comp);
    }
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline atVec3f NodeToVal(const YAMLNode* node)
{
    return NodeToVec<atVec3f>(node);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atVec3f& val)
{
    YAMLNode* ret = new YAMLNode(YAML_SEQUENCE_NODE);
    ret->m_seqChildren.reserve(3);
    for (size_t i=0 ; i<3 ; ++i)
    {
        char str[64];
        snprintf(str, 64, "%f", val.vec[i]);
        YAMLNode* comp = new YAMLNode(YAML_SCALAR_NODE);
        comp->m_scalarString = str;
        ret->m_seqChildren.emplace_back(comp);
    }
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline atVec4f NodeToVal(const YAMLNode* node)
{
    return NodeToVec<atVec4f>(node);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const atVec4f& val)
{
    YAMLNode* ret = new YAMLNode(YAML_SEQUENCE_NODE);
    ret->m_seqChildren.reserve(4);
    for (size_t i=0 ; i<4 ; ++i)
    {
        char str[64];
        snprintf(str, 64, "%f", val.vec[i]);
        YAMLNode* comp = new YAMLNode(YAML_SCALAR_NODE);
        comp->m_scalarString = str;
        ret->m_seqChildren.emplace_back(comp);
    }
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline std::unique_ptr<atUint8[]> NodeToVal(const YAMLNode* node)
{
    return base64_decode(node->m_scalarString);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const std::unique_ptr<atUint8[]>& val, size_t byteCount)
{
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = base64_encode(val.get(), byteCount);
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline std::string NodeToVal(const YAMLNode* node)
{
    return node->m_scalarString;
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const std::string& val)
{
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = val;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const char* val)
{
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString = val;
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline std::wstring NodeToVal(const YAMLNode* node)
{
    std::wstring retval;
    retval.reserve(node->m_scalarString.length());
    const char* buf = node->m_scalarString.c_str();
    while (*buf)
    {
        wchar_t wc;
        buf += std::mbtowc(&wc, buf, MB_CUR_MAX);
        retval += wc;
    }
    return retval;
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const std::wstring& val)
{
    YAMLNode* ret = new YAMLNode(YAML_SCALAR_NODE);
    ret->m_scalarString.reserve(val.length());
    for (wchar_t ch : val)
    {
        char mb[4];
        int c = std::wctomb(mb, ch);
        ret->m_scalarString.append(mb, c);
    }
    return std::unique_ptr<YAMLNode>(ret);
}

template <>
inline std::unique_ptr<YAMLNode> ValToNode(const wchar_t* val)
{
    std::wstring wstr(val);
    return ValToNode<const std::wstring&>(wstr);
}

class YAMLDocReader
{
    std::unique_ptr<YAMLNode> m_rootNode;
    std::vector<YAMLNode*> m_subStack;
    std::vector<int> m_seqTrackerStack;
    static std::unique_ptr<YAMLNode> ParseEvents(yaml_parser_t* doc);
public:
    inline const YAMLNode* getRootNode() const {return m_rootNode.get();}
    bool read(yaml_parser_t* doc)
    {
        std::unique_ptr<YAMLNode> newRoot = ParseEvents(doc);
        if (!newRoot)
            return false;
        m_rootNode = std::move(newRoot);
        m_subStack.clear();
        m_subStack.push_back(m_rootNode.get());
        m_seqTrackerStack.clear();
        return true;
    }

    void enterSubRecord(const char* name)
    {
        YAMLNode* curSub = m_subStack.back();
        if (curSub->m_type == YAML_SEQUENCE_NODE)
        {
            int& seqIdx = m_seqTrackerStack.back();
            m_subStack.push_back(curSub->m_seqChildren[seqIdx++].get());
            return;
        }
        for (const auto& item : curSub->m_mapChildren)
        {
            if (!item.first.compare(name))
            {
                m_subStack.push_back(item.second.get());
                return;
            }
        }
    }

    void leaveSubRecord()
    {
        if (m_subStack.size() > 1)
            m_subStack.pop_back();
    }

    template <class T>
    void enumerate(const char* name, T& record)
    {
        enterSubRecord(name);
        record.fromYAML(*this);
        leaveSubRecord();
    }

    void enterSubVector(const char* name)
    {
        YAMLNode* curSub = m_subStack.back();
        for (const auto& item : curSub->m_mapChildren)
        {
            if (!item.first.compare(name))
            {
                YAMLNode* nextSub = item.second.get();
                m_subStack.push_back(nextSub);
                m_seqTrackerStack.push_back(0);
                break;
            }
        }
    }

    void leaveSubVector()
    {
        if (m_subStack.size() > 1)
        {
            m_subStack.pop_back();
            m_seqTrackerStack.pop_back();
        }
    }

    template <class T>
    void enumerate(const char* name, std::vector<T>& vector, size_t count,
                   typename std::enable_if<!std::is_arithmetic<T>::value &&
                                           !std::is_same<T, atVec2f>::value &&
                                           !std::is_same<T, atVec3f>::value &&
                                           !std::is_same<T, atVec4f>::value>::type* = 0)
    {
        vector.clear();
        vector.reserve(count);
        enterSubVector(name);
        for (size_t i=0 ; i<count ; ++i)
        {
            vector.emplace_back();
            enterSubRecord(nullptr);
            vector.back().fromYAML(*this);
            leaveSubRecord();
        }
        leaveSubVector();
    }

    template <class T>
    void enumerate(const char* name, std::vector<T>& vector, size_t count,
                   typename std::enable_if<std::is_arithmetic<T>::value ||
                                           std::is_same<T, atVec2f>::value ||
                                           std::is_same<T, atVec3f>::value ||
                                           std::is_same<T, atVec4f>::value>::type* = 0)
    {
        vector.clear();
        vector.reserve(count);
        enterSubVector(name);
        for (size_t i=0 ; i<count ; ++i)
            vector.emplace_back(readVal<T>(name));
        leaveSubVector();
    }

    template <class T>
    void enumerate(const char* name, std::vector<T>& vector, size_t count,
                   std::function<void(YAMLDocReader&, T&)> readf)
    {
        vector.clear();
        vector.reserve(count);
        enterSubVector(name);
        for (size_t i=0 ; i<count ; ++i)
        {
            vector.emplace_back();
            enterSubRecord(nullptr);
            readf(*this, vector.back());
            leaveSubRecord();
        }
        leaveSubVector();
    }

    template <typename RETURNTYPE>
    RETURNTYPE readVal(const char* name)
    {
        if (m_subStack.size())
        {
            const YAMLNode* mnode = m_subStack.back();
            if (mnode->m_type == YAML_SCALAR_NODE)
            {
                return NodeToVal<RETURNTYPE>(mnode);
            }
            else if (mnode->m_type == YAML_SEQUENCE_NODE)
            {
                int& seqIdx = m_seqTrackerStack.back();
                return NodeToVal<RETURNTYPE>(mnode->m_seqChildren[seqIdx++].get());
            }
            else if (mnode->m_type == YAML_MAPPING_NODE)
            {
                for (const auto& item : mnode->m_mapChildren)
                {
                    if (!item.first.compare(name))
                    {
                        return NodeToVal<RETURNTYPE>(item.second.get());
                    }
                }
            }
        }
        atWarning("Unable to find field '%s'; returning 0", name);
        return RETURNTYPE();
    }

    inline bool readBool(const char* name)
    {
        return readVal<bool>(name);
    }

    inline atInt8 readByte(const char* name)
    {
        return readVal<atInt8>(name);
    }

    inline atUint8 readUByte(const char* name)
    {
        return readVal<atUint8>(name);
    }

    inline atInt16 readInt16(const char* name)
    {
        return readVal<atInt16>(name);
    }

    inline atUint16 readUint16(const char* name)
    {
        return readVal<atUint16>(name);
    }

    inline atInt32 readInt32(const char* name)
    {
        return readVal<atInt32>(name);
    }

    inline atUint32 readUint32(const char* name)
    {
        return readVal<atUint32>(name);
    }

    inline atInt64 readInt64(const char* name)
    {
        return readVal<atInt64>(name);
    }

    inline atUint64 readUint64(const char* name)
    {
        return readVal<atUint64>(name);
    }

    inline float readFloat(const char* name)
    {
        return readVal<float>(name);
    }

    inline double readDouble(const char* name)
    {
        return readVal<double>(name);
    }

    inline atVec2f readVec2f(const char* name)
    {
        return readVal<atVec2f>(name);
    }

    inline atVec3f readVec3f(const char* name)
    {
        return readVal<atVec3f>(name);
    }

    inline atVec4f readVec4f(const char* name)
    {
        return readVal<atVec4f>(name);
    }

    inline std::unique_ptr<atUint8[]> readUBytes(const char* name)
    {
        return readVal<std::unique_ptr<atUint8[]>>(name);
    }

    inline std::string readString(const char* name)
    {
        return readVal<std::string>(name);
    }

    inline std::wstring readWString(const char* name)
    {
        return readVal<std::wstring>(name);
    }

};

class YAMLDocWriter
{
    YAMLNode m_rootNode;
    std::vector<YAMLNode*> m_subStack;
    static bool RecursiveFinish(yaml_emitter_t* doc, const YAMLNode& node);
public:
    YAMLDocWriter() : m_rootNode(YAML_MAPPING_NODE)
    {
        m_subStack.emplace_back(&m_rootNode);
    }

    void enterSubRecord(const char* name)
    {
        YAMLNode* curSub = m_subStack.back();
        if (curSub->m_type != YAML_MAPPING_NODE &&
            curSub->m_type != YAML_SEQUENCE_NODE)
            return;
        YAMLNode* newNode = new YAMLNode(YAML_MAPPING_NODE);
        if (curSub->m_type == YAML_MAPPING_NODE)
            curSub->m_mapChildren.emplace_back(name?std::string(name):std::string(), std::unique_ptr<YAMLNode>(newNode));
        else if (curSub->m_type == YAML_SEQUENCE_NODE)
            curSub->m_seqChildren.emplace_back(newNode);
        m_subStack.push_back(newNode);
    }

    void leaveSubRecord()
    {
        if (m_subStack.size() > 1)
        {
            YAMLNode* curSub = m_subStack.back();
            /* Automatically lower to scalar if there's only one unnamed node */
            if (curSub->m_mapChildren.size() == 1 &&
                curSub->m_mapChildren[0].first.empty())
            {
                auto& item = curSub->m_mapChildren[0];
                if (item.first.empty() && item.second->m_type == YAML_SCALAR_NODE)
                {
                    curSub->m_type = YAML_SCALAR_NODE;
                    curSub->m_scalarString = std::move(item.second->m_scalarString);
                    curSub->m_mapChildren.clear();
                }
            }
            m_subStack.pop_back();
        }
    }

    template <class T>
    void enumerate(const char* name, T& record)
    {
        enterSubRecord(name);
        record.toYAML(*this);
        leaveSubRecord();
    }

    void enterSubVector(const char* name)
    {
        YAMLNode* curSub = m_subStack.back();
        if (curSub->m_type != YAML_MAPPING_NODE &&
            curSub->m_type != YAML_SEQUENCE_NODE)
            return;
        YAMLNode* newNode = new YAMLNode(YAML_SEQUENCE_NODE);
        if (curSub->m_type == YAML_MAPPING_NODE)
            curSub->m_mapChildren.emplace_back(name?std::string(name):std::string(), std::unique_ptr<YAMLNode>(newNode));
        else if (curSub->m_type == YAML_SEQUENCE_NODE)
            curSub->m_seqChildren.emplace_back(newNode);
        m_subStack.push_back(newNode);
    }

    void leaveSubVector()
    {
        if (m_subStack.size() > 1)
            m_subStack.pop_back();
    }

    template <class T>
    void enumerate(const char* name, const std::vector<T>& vector,
                   typename std::enable_if<!std::is_arithmetic<T>::value &&
                                           !std::is_same<T, atVec2f>::value &&
                                           !std::is_same<T, atVec3f>::value &&
                                           !std::is_same<T, atVec4f>::value>::type* = 0)
    {
        enterSubVector(name);
        for (const T& item : vector)
        {
            enterSubRecord(nullptr);
            item.toYAML(*this);
            leaveSubRecord();
        }
        leaveSubVector();
    }

    template <class T>
    void enumerate(const char* name, const std::vector<T>& vector,
                   typename std::enable_if<std::is_arithmetic<T>::value ||
                                           std::is_same<T, atVec2f>::value ||
                                           std::is_same<T, atVec3f>::value ||
                                           std::is_same<T, atVec4f>::value>::type* = 0)
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

    bool finish(yaml_emitter_t* docOut)
    {
        yaml_event_t event = {YAML_DOCUMENT_START_EVENT};
        event.data.document_start.implicit = true;
        if (!yaml_emitter_emit(docOut, &event))
            goto err;
        if (!RecursiveFinish(docOut, m_rootNode))
            return false;
        event.type = YAML_DOCUMENT_END_EVENT;
        event.data.document_end.implicit = true;
        if (!yaml_emitter_emit(docOut, &event))
            goto err;
        return true;
    err:
        HandleYAMLEmitterError(docOut);
        return false;
    }

    template <typename INTYPE>
    void writeVal(const char* name, INTYPE val)
    {
        YAMLNode* curSub = m_subStack.back();
        if (curSub->m_type == YAML_MAPPING_NODE)
            curSub->m_mapChildren.emplace_back(name?name:std::string(), std::move(ValToNode(val)));
        else if (curSub->m_type == YAML_SEQUENCE_NODE)
            curSub->m_seqChildren.emplace_back(std::move(ValToNode(val)));
    }

    template <typename INTYPE>
    void writeVal(const char* name, INTYPE val, size_t byteCount)
    {
        YAMLNode* curSub = m_subStack.back();
        if (curSub->m_type == YAML_MAPPING_NODE)
            curSub->m_mapChildren.emplace_back(name?name:std::string(), std::move(ValToNode(val, byteCount)));
        else if (curSub->m_type == YAML_SEQUENCE_NODE)
            curSub->m_seqChildren.emplace_back(std::move(ValToNode(val, byteCount)));
    }

    inline void writeBool(const char* name, const bool& val)
    {
        writeVal<bool>(name, val);
    }

    inline void writeByte(const char* name, const atInt8& val)
    {
        writeVal<atInt8>(name, val);
    }

    inline void writeUByte(const char* name, const atUint8& val)
    {
        writeVal<atUint8>(name, val);
    }

    inline void writeInt16(const char* name, const atInt16& val)
    {
        writeVal<atInt16>(name, val);
    }

    inline void writeUint16(const char* name, const atUint16& val)
    {
        writeVal<atUint16>(name, val);
    }

    inline void writeInt32(const char* name, const atInt32& val)
    {
        writeVal<atInt32>(name, val);
    }

    inline void writeUint32(const char* name, const atUint32& val)
    {
        writeVal<atUint32>(name, val);
    }

    inline void writeInt64(const char* name, const atInt64& val)
    {
        writeVal<atInt64>(name, val);
    }

    inline void writeUint64(const char* name, const atUint64& val)
    {
        writeVal<atUint64>(name, val);
    }

    inline void writeFloat(const char* name, const float& val)
    {
        writeVal<float>(name, val);
    }

    inline void writeDouble(const char* name, const double& val)
    {
        writeVal<double>(name, val);
    }

    inline void writeVec2f(const char* name, const atVec2f& val)
    {
        writeVal<atVec2f>(name, val);
    }

    inline void writeVec3f(const char* name, const atVec3f& val)
    {
        writeVal<atVec3f>(name, val);
    }

    inline void writeVec4f(const char* name, const atVec4f& val)
    {
        writeVal<atVec4f>(name, val);
    }

    inline void writeUBytes(const char* name, const std::unique_ptr<atUint8[]>& val, size_t byteCount)
    {
        writeVal<const std::unique_ptr<atUint8[]>&>(name, val, byteCount);
    }

    inline void writeString(const char* name, const std::string& val)
    {
        writeVal<std::string>(name, val);
    }

    inline void writeString(const char* name, const char* val)
    {
        writeVal<const char*>(name, val);
    }

    inline void writeWString(const char* name, const std::wstring& val)
    {
        writeVal<std::wstring>(name, val);
    }

    inline void writeWString(const char* name, const wchar_t* val)
    {
        writeVal<const wchar_t*>(name, val);
    }
};

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
    virtual void toYAML(YAMLDocWriter& out) const=0;
    virtual void fromYAML(YAMLDocReader& in)=0;

    template <size_t sizeVar>
    using Buffer = struct Athena::io::BufferYaml<sizeVar, DNAE>;

    template <atInt32 sizeVar = -1>
    using String = struct Athena::io::StringYaml<sizeVar, DNAE>;

    template <atInt32 sizeVar = -1, Endian VE = DNAE>
    using WString = struct Athena::io::WStringYaml<sizeVar, VE>;

    template <atInt32 sizeVar = -1>
    using WStringAsString = struct Athena::io::WStringAsStringYaml<sizeVar, DNAE>;

    std::string toYAMLString() const
    {
        yaml_emitter_t emitter;
        if (!yaml_emitter_initialize(&emitter))
        {
            HandleYAMLEmitterError(&emitter);
            return std::string();
        }

        std::string res;
        yaml_emitter_set_output(&emitter, (yaml_write_handler_t*)YAMLStdStringWriter, &res);
        yaml_emitter_set_unicode(&emitter, true);
        yaml_emitter_set_width(&emitter, -1);

        if (!yaml_emitter_open(&emitter))
        {
            HandleYAMLEmitterError(&emitter);
            yaml_emitter_delete(&emitter);
            return std::string();
        }
        {
            YAMLDocWriter docWriter;
            toYAML(docWriter);
            if (!docWriter.finish(&emitter))
            {
                yaml_emitter_delete(&emitter);
                return std::string();
            }
        }
        if (!yaml_emitter_close(&emitter) ||
            !yaml_emitter_flush(&emitter))
        {
            HandleYAMLEmitterError(&emitter);
            yaml_emitter_delete(&emitter);
            return std::string();
        }
        yaml_emitter_delete(&emitter);

        return res;
    }

    bool fromYAMLString(const std::string& str)
    {
        yaml_parser_t parser;
        if (!yaml_parser_initialize(&parser))
        {
            HandleYAMLParserError(&parser);
            return false;
        }

        YAMLStdStringReaderState reader(str);
        yaml_parser_set_input(&parser, (yaml_read_handler_t*)YAMLStdStringReader, &reader);

        YAMLDocReader docReader;
        if (!docReader.read(&parser))
        {
            yaml_parser_delete(&parser);
            return false;
        }
        fromYAML(docReader);
        yaml_parser_delete(&parser);
        return true;
    }

    bool toYAMLFile(FILE* fout) const
    {
        yaml_emitter_t emitter;
        if (!yaml_emitter_initialize(&emitter))
        {
            HandleYAMLEmitterError(&emitter);
            return false;
        }

        yaml_emitter_set_output_file(&emitter, fout);
        yaml_emitter_set_unicode(&emitter, true);
        yaml_emitter_set_width(&emitter, -1);

        if (!yaml_emitter_open(&emitter))
        {
            HandleYAMLEmitterError(&emitter);
            yaml_emitter_delete(&emitter);
            return false;
        }
        {
            YAMLDocWriter docWriter;
            toYAML(docWriter);
            if (!docWriter.finish(&emitter))
            {
                yaml_emitter_delete(&emitter);
                return false;
            }
        }
        if (!yaml_emitter_close(&emitter) ||
            !yaml_emitter_flush(&emitter))
        {
            HandleYAMLEmitterError(&emitter);
            yaml_emitter_delete(&emitter);
            return false;
        }
        yaml_emitter_delete(&emitter);

        return true;
    }

    bool fromYAMLFile(FILE* fin)
    {
        yaml_parser_t parser;
        if (!yaml_parser_initialize(&parser))
        {
            HandleYAMLParserError(&parser);
            return false;
        }

        yaml_parser_set_input_file(&parser, fin);

        YAMLDocReader docReader;
        if (!docReader.read(&parser))
        {
            yaml_parser_delete(&parser);
            return false;
        }
        fromYAML(docReader);
        yaml_parser_delete(&parser);
        return true;
    }
};

template <size_t sizeVar, Endian VE>
struct BufferYaml : public DNAYaml<VE>, public std::unique_ptr<atUint8[]>
{
    typename DNA<VE>::Delete expl;
    inline void read(IStreamReader& reader)
    {
        reset(new atUint8[sizeVar]);
        reader.readUBytesToBuf(get(), sizeVar);
    }
    inline void write(IStreamWriter& writer) const
    {
        writer.writeUBytes(get(), sizeVar);
    }
    inline void fromYAML(Athena::io::YAMLDocReader& reader)
    {*this = reader.readUBytes(nullptr);}
    inline void toYAML(Athena::io::YAMLDocWriter& writer) const
    {writer.writeUBytes(nullptr, *this, sizeVar);}
};

template <atInt32 sizeVar, Endian VE>
struct StringYaml : public DNAYaml<VE>, public std::string
{
    typename DNA<VE>::Delete expl;
    inline void read(IStreamReader& reader)
    {this->assign(std::move(reader.readString(sizeVar)));}
    inline void write(IStreamWriter& writer) const
    {writer.writeString(*this, sizeVar);}
    inline void fromYAML(Athena::io::YAMLDocReader& reader)
    {this->assign(std::move(reader.readString(nullptr)));}
    inline void toYAML(Athena::io::YAMLDocWriter& writer) const
    {writer.writeString(nullptr, *this);}
    inline std::string& operator=(const std::string& __str)
    {return this->assign(__str);}
    inline std::string& operator=(std::string&& __str)
    {this->swap(__str); return *this;}
};

template <atInt32 sizeVar, Endian VE>
struct WStringYaml : public DNAYaml<VE>, public std::wstring
{
    typename DNA<VE>::Delete expl;
    inline void read(IStreamReader& reader)
    {
        reader.setEndian(VE);
        this->assign(std::move(reader.readWString(sizeVar)));
    }
    inline void write(IStreamWriter& writer) const
    {
        writer.setEndian(VE);
        writer.writeWString(*this, sizeVar);
    }
    inline void fromYAML(Athena::io::YAMLDocReader& reader)
    {this->assign(std::move(reader.readWString(nullptr)));}
    inline void toYAML(Athena::io::YAMLDocWriter& writer) const
    {writer.writeWString(nullptr, *this);}
    inline std::wstring& operator=(const std::wstring& __str)
    {return this->assign(__str);}
    inline std::wstring& operator=(std::wstring&& __str)
    {this->swap(__str); return *this;}
};

template <atInt32 sizeVar, Endian VE>
struct WStringAsStringYaml : public DNAYaml<VE>, public std::string
{
    typename DNA<VE>::Delete expl;
    inline void read(IStreamReader& reader)
    {*this = reader.readWStringAsString(sizeVar);}
    inline void write(IStreamWriter& writer) const
    {writer.writeStringAsWString(*this, sizeVar);}
    inline void fromYAML(Athena::io::YAMLDocReader& reader)
    {this->assign(std::move(reader.readString(nullptr)));}
    inline void toYAML(Athena::io::YAMLDocWriter& writer) const
    {writer.writeString(nullptr, *this);}
    inline std::string& operator=(const std::string& __str)
    {return this->assign(__str);}
    inline std::string& operator=(std::string&& __str)
    {this->swap(__str); return *this;}
};

/** Macro to automatically declare YAML read/write methods in subclasses */
#define DECL_YAML \
    DECL_DNA \
    void fromYAML(Athena::io::YAMLDocReader&); \
    void toYAML(Athena::io::YAMLDocWriter&) const; \

/** Macro to automatically declare YAML read/write methods with client-code's definition */
#define DECL_EXPLICIT_YAML \
    void fromYAML(Athena::io::YAMLDocReader&); \
    void toYAML(Athena::io::YAMLDocWriter&) const; \

}
}

#endif // DNAYAML_HPP
