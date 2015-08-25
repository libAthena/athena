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

extern const char* AT_BASE64_CHARS;

static inline bool at_is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

template<typename STLTRAITS>
typename STLTRAITS::String base64_encode(const atUint8* bytes_to_encode, size_t in_len)
{
    typename STLTRAITS::String ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    ret.reserve(in_len * 4 / 3);

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += AT_BASE64_CHARS[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += AT_BASE64_CHARS[char_array_4[j]];

        while((i++ < 3))
            ret += '=';

    }

    return ret;

}

template<typename STLTRAITS>
std::unique_ptr<atUint8[]> base64_decode(const typename STLTRAITS::String& encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::unique_ptr<atUint8[]> ret(new atUint8[in_len * 3 / 4]);
    atUint8* retBuf = ret.get();

    while (in_len-- && ( encoded_string[in_] != '=') && at_is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = reinterpret_cast<uintptr_t>(strchr(AT_BASE64_CHARS, char_array_4[i]) - char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                *retBuf++ = char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = reinterpret_cast<uintptr_t>(strchr(AT_BASE64_CHARS, char_array_4[j]) - char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) *retBuf++ = char_array_3[j];
    }

    return ret;
}

void HandleYAMLParserError(yaml_parser_t* parser);
void HandleYAMLEmitterError(yaml_emitter_t* emitter);

template<typename STLTRAITS>
struct YAMLStdStringReaderState
{
    typename STLTRAITS::String::const_iterator begin;
    typename STLTRAITS::String::const_iterator end;
    YAMLStdStringReaderState(const typename STLTRAITS::String& str)
    {
        begin = str.begin();
        end = str.end();
    }
};

template<typename STLTRAITS>
int YAMLStdStringReader(YAMLStdStringReaderState<STLTRAITS>* reader,
                        unsigned char* buffer, size_t size, size_t* size_read)
{
    size_t diff = reader->end - reader->begin;
    if (!diff)
    {
        *size_read = 0;
    }
    else if (diff < size)
    {
        memcpy(buffer, &*reader->begin, diff);
        *size_read = diff;
    }
    else
    {
        memcpy(buffer, &*reader->begin, size);
        *size_read = size;
    }
    return 1;
}

template<typename STLTRAITS>
int YAMLStdStringWriter(typename STLTRAITS::String* str, unsigned char *buffer, size_t size)
{
    str->append((char*)buffer, size);
    return 1;
}

template<typename STLTRAITS>
struct YAMLNode
{
    yaml_node_type_t m_type;
    typename STLTRAITS::String m_scalarString;
    typename STLTRAITS::template Vector<std::unique_ptr<YAMLNode>> m_seqChildren;
    typename STLTRAITS::template Vector<std::pair<typename STLTRAITS::String, std::unique_ptr<YAMLNode>>> m_mapChildren;
    YAMLNode(yaml_node_type_t type) : m_type(type) {}
    inline const YAMLNode* findMapChild(const char* key) const
    {
        for (const auto& item : m_mapChildren)
            if (!item.first.compare(key))
                return item.second.get();
        return nullptr;
    }
};

template<typename STLTRAITS>
static inline void InsertNode(typename STLTRAITS::template Vector<YAMLNode<STLTRAITS>*>& nodeStack,
                              std::unique_ptr<YAMLNode<STLTRAITS>>& mapKey,
                              std::unique_ptr<YAMLNode<STLTRAITS>>& retVal,
                              std::unique_ptr<YAMLNode<STLTRAITS>>&& newNode)
{
    if (nodeStack.empty())
    {
        retVal = std::move(newNode);
        return;
    }
    YAMLNode<STLTRAITS>* parent = nodeStack.back();
    if (parent->m_type == YAML_SEQUENCE_NODE)
    {
        parent->m_seqChildren.emplace_back(std::move(newNode));
    }
    else if (parent->m_type == YAML_MAPPING_NODE)
    {
        if (!mapKey)
            mapKey = std::move(newNode);
        else
        {
            parent->m_mapChildren.emplace_back(std::move(mapKey->m_scalarString), std::move(newNode));
            mapKey.reset(nullptr);
        }
    }
}

static inline bool EmitKeyScalar(yaml_emitter_t* doc, const char* val)
{
    yaml_event_t event;
    if (!yaml_scalar_event_initialize(&event, nullptr, nullptr, (yaml_char_t*)val,
                                      strlen(val), true, true, YAML_PLAIN_SCALAR_STYLE))
        return false;
    return yaml_emitter_emit(doc, &event);
}

template<typename STLTRAITS>
static inline yaml_scalar_style_t ScalarStyle(const YAMLNode<STLTRAITS>& node)
{
    for (const auto& ch : node.m_scalarString)
        if (ch == '\n')
            return YAML_LITERAL_SCALAR_STYLE;
    return YAML_ANY_SCALAR_STYLE;
}

template<typename STLTRAITS>
static inline yaml_sequence_style_t SequenceStyle(const YAMLNode<STLTRAITS>& node)
{
    size_t count = 0;
    for (const auto& item : node.m_seqChildren)
    {
        if (item->m_type != YAML_SCALAR_NODE)
            return YAML_BLOCK_SEQUENCE_STYLE;
        size_t strLen = item->m_scalarString.size();
        size_t thisCount = strLen / 10;
        if (!thisCount)
            thisCount = 1;
        count += thisCount;
    }
    return (count > 6) ? YAML_BLOCK_SEQUENCE_STYLE : YAML_FLOW_SEQUENCE_STYLE;
}

template<typename STLTRAITS>
static inline yaml_mapping_style_t MappingStyle(const YAMLNode<STLTRAITS>& node)
{
    size_t count = 0;
    for (const auto& item : node.m_mapChildren)
    {
        if (item.second->m_type != YAML_SCALAR_NODE)
            return YAML_BLOCK_MAPPING_STYLE;
        size_t strLen = item.second->m_scalarString.size();
        size_t thisCount = strLen / 10;
        if (!thisCount)
            thisCount = 1;
        count += thisCount;
    }
    return (count > 6) ? YAML_BLOCK_MAPPING_STYLE : YAML_FLOW_MAPPING_STYLE;
}

template<typename STLTRAITS>
struct YAMLTranslate
{
    template <typename INTYPE>
    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const INTYPE& val);
    template <typename INTYPE>
    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const INTYPE* val);
    template <typename INTYPE>
    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const INTYPE& val, size_t byteCount);

    template<typename RETURNTYPE>
    static bool NodeToVal(const YAMLNode<STLTRAITS>* node,
                          typename std::enable_if<std::is_same<bool, RETURNTYPE>::value>::type* = 0)
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

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const bool& val)
    {
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = val?"True":"False";
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static atInt8 NodeToVal(const YAMLNode<STLTRAITS>* node,
                            typename std::enable_if<std::is_same<atInt8, RETURNTYPE>::value>::type* = 0)
    {
        return strtol(node->m_scalarString.c_str(), NULL, 0);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atInt8& val)
    {
        char str[32];
        snprintf(str, 32, "0x%02X", val);
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = str;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static atUint8 NodeToVal(const YAMLNode<STLTRAITS>* node,
                             typename std::enable_if<std::is_same<atUint8, RETURNTYPE>::value>::type* = 0)
    {
        return strtoul(node->m_scalarString.c_str(), NULL, 0);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atUint8& val)
    {
        char str[32];
        snprintf(str, 32, "0x%02X", val);
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = str;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static atInt16 NodeToVal(const YAMLNode<STLTRAITS>* node,
                             typename std::enable_if<std::is_same<atInt16, RETURNTYPE>::value>::type* = 0)
    {
        return strtol(node->m_scalarString.c_str(), NULL, 0);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atInt16& val)
    {
        char str[32];
        snprintf(str, 32, "0x%04X", val);
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = str;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static atUint16 NodeToVal(const YAMLNode<STLTRAITS>* node,
                              typename std::enable_if<std::is_same<atUint16, RETURNTYPE>::value>::type* = 0)
    {
        return strtoul(node->m_scalarString.c_str(), NULL, 0);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atUint16& val)
    {
        char str[32];
        snprintf(str, 32, "0x%04X", val);
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = str;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static atInt32 NodeToVal(const YAMLNode<STLTRAITS>* node,
                             typename std::enable_if<std::is_same<atInt32, RETURNTYPE>::value>::type* = 0)
    {
        return strtol(node->m_scalarString.c_str(), NULL, 0);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atInt32& val)
    {
        char str[32];
        snprintf(str, 32, "0x%08X", val);
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = str;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static atUint32 NodeToVal(const YAMLNode<STLTRAITS>* node,
                              typename std::enable_if<std::is_same<atUint32, RETURNTYPE>::value>::type* = 0)
    {
        return strtoul(node->m_scalarString.c_str(), NULL, 0);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atUint32& val)
    {
        char str[32];
        snprintf(str, 32, "0x%08X", val);
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = str;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static atInt64 NodeToVal(const YAMLNode<STLTRAITS>* node,
                             typename std::enable_if<std::is_same<atInt64, RETURNTYPE>::value>::type* = 0)
    {
#if _WIN32
        return _strtoi64(node->m_scalarString.c_str(), NULL, 0);
#else
        return strtoq(node->m_scalarString.c_str(), NULL, 0);
#endif
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atInt64& val)
    {
        char str[32];
        snprintf(str, 32, "0x%016llX", val);
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = str;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static atUint64 NodeToVal(const YAMLNode<STLTRAITS>* node,
                              typename std::enable_if<std::is_same<atUint64, RETURNTYPE>::value>::type* = 0)
    {
#if _WIN32
        return _strtoui64(node->m_scalarString.c_str(), NULL, 0);
#else
        return strtouq(node->m_scalarString.c_str(), NULL, 0);
#endif
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atUint64& val)
    {
        char str[32];
        snprintf(str, 32, "0x%016llX", val);
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = str;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static float NodeToVal(const YAMLNode<STLTRAITS>* node,
                           typename std::enable_if<std::is_same<float, RETURNTYPE>::value>::type* = 0)
    {
        return strtof(node->m_scalarString.c_str(), NULL);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const float& val)
    {
        char str[64];
        snprintf(str, 64, "%f", val);
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = str;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static double NodeToVal(const YAMLNode<STLTRAITS>* node,
                            typename std::enable_if<std::is_same<double, RETURNTYPE>::value>::type* = 0)
    {
        return strtod(node->m_scalarString.c_str(), NULL);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const double& val)
    {
        char str[64];
        snprintf(str, 64, "%f", val);
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = str;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template <typename RETURNTYPE>
    static RETURNTYPE NodeToVec(const YAMLNode<STLTRAITS>* node)
    {
        RETURNTYPE retval = {};
        auto it = node->m_seqChildren.begin();
        for (size_t i=0;
             i<4 && it != node->m_seqChildren.end();
             ++i, ++it)
        {
            YAMLNode<STLTRAITS>* snode = it->get();
            if (snode->m_type == YAML_SCALAR_NODE)
                retval.vec[i] = NodeToVal<float>(snode);
            else
                retval.vec[i] = 0.0;
        }
        return retval;
    }

    template<typename RETURNTYPE>
    static atVec2f NodeToVal(const YAMLNode<STLTRAITS>* node,
                             typename std::enable_if<std::is_same<atVec2f, RETURNTYPE>::value>::type* = 0)
    {
        return NodeToVec<atVec2f>(node);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atVec2f& val)
    {
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SEQUENCE_NODE);
        ret->m_seqChildren.reserve(2);
        for (size_t i=0 ; i<2 ; ++i)
        {
            char str[64];
            snprintf(str, 64, "%f", val.vec[i]);
            YAMLNode<STLTRAITS>* comp = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
            comp->m_scalarString = str;
            ret->m_seqChildren.emplace_back(comp);
        }
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static atVec3f NodeToVal(const YAMLNode<STLTRAITS>* node,
                             typename std::enable_if<std::is_same<atVec3f, RETURNTYPE>::value>::type* = 0)
    {
        return NodeToVec<atVec3f>(node);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atVec3f& val)
    {
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SEQUENCE_NODE);
        ret->m_seqChildren.reserve(3);
        for (size_t i=0 ; i<3 ; ++i)
        {
            char str[64];
            snprintf(str, 64, "%f", val.vec[i]);
            YAMLNode<STLTRAITS>* comp = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
            comp->m_scalarString = str;
            ret->m_seqChildren.emplace_back(comp);
        }
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static atVec4f NodeToVal(const YAMLNode<STLTRAITS>* node,
                             typename std::enable_if<std::is_same<atVec4f, RETURNTYPE>::value>::type* = 0)
    {
        return NodeToVec<atVec4f>(node);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const atVec4f& val)
    {
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SEQUENCE_NODE);
        ret->m_seqChildren.reserve(4);
        for (size_t i=0 ; i<4 ; ++i)
        {
            char str[64];
            snprintf(str, 64, "%f", val.vec[i]);
            YAMLNode<STLTRAITS>* comp = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
            comp->m_scalarString = str;
            ret->m_seqChildren.emplace_back(comp);
        }
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static std::unique_ptr<atUint8[]> NodeToVal(const YAMLNode<STLTRAITS>* node,
                                                typename std::enable_if<std::is_same<std::unique_ptr<atUint8[]>, RETURNTYPE>::value>::type* = 0)
    {
        return base64_decode(node->m_scalarString);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const std::unique_ptr<atUint8[]>& val, size_t byteCount)
    {
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = base64_encode<STLTRAITS>(val.get(), byteCount);
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static typename STLTRAITS::String NodeToVal(const YAMLNode<STLTRAITS>* node,
                                                typename std::enable_if<std::is_same<typename STLTRAITS::String, RETURNTYPE>::value>::type* = 0)
    {
        return node->m_scalarString;
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const typename STLTRAITS::String& val)
    {
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = val;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const char* val)
    {
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString = val;
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    template<typename RETURNTYPE>
    static typename STLTRAITS::WString NodeToVal(const YAMLNode<STLTRAITS>* node,
                                                 typename std::enable_if<std::is_same<typename STLTRAITS::WString, RETURNTYPE>::value>::type* = 0)
    {
        typename STLTRAITS::WString retval;
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

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const typename STLTRAITS::WString& val)
    {
        YAMLNode<STLTRAITS>* ret = new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE);
        ret->m_scalarString.reserve(val.length());
        for (wchar_t ch : val)
        {
            char mb[4];
            int c = std::wctomb(mb, ch);
            ret->m_scalarString.append(mb, c);
        }
        return std::unique_ptr<YAMLNode<STLTRAITS>>(ret);
    }

    static std::unique_ptr<YAMLNode<STLTRAITS>> ValToNode(const wchar_t* val)
    {
        std::wstring wstr(val);
        return ValToNode<const typename STLTRAITS::WString&>(wstr);
    }

};

template <typename STLTRAITS>
class YAMLDocReader
{
    std::unique_ptr<YAMLNode<STLTRAITS>> m_rootNode;
    typename STLTRAITS::template Vector<YAMLNode<STLTRAITS>*> m_subStack;
    typename STLTRAITS::template Vector<int> m_seqTrackerStack;
    static std::unique_ptr<YAMLNode<STLTRAITS>> ParseEvents(yaml_parser_t* doc)
    {
        yaml_event_t event;
        if (!yaml_parser_parse(doc, &event))
        {
            HandleYAMLParserError(doc);
            return std::unique_ptr<YAMLNode<STLTRAITS>>();
        }

        typename STLTRAITS::template Vector<YAMLNode<STLTRAITS>*> nodeStack;
        std::unique_ptr<YAMLNode<STLTRAITS>> mapKey;
        std::unique_ptr<YAMLNode<STLTRAITS>> retVal;
        int result;
        for (result = yaml_parser_parse(doc, &event);
             event.type != YAML_STREAM_END_EVENT;
             result = yaml_parser_parse(doc, &event))
        {
            if (!result)
            {
                HandleYAMLParserError(doc);
                return std::unique_ptr<YAMLNode<STLTRAITS>>();
            }
            switch (event.type)
            {
            case YAML_SCALAR_EVENT:
            {
                if (nodeStack.empty())
                {
                    atWarning("YAML parser stack empty; skipping scalar node");
                    break;
                }
                std::unique_ptr<YAMLNode<STLTRAITS>> newScalar(new YAMLNode<STLTRAITS>(YAML_SCALAR_NODE));
                newScalar->m_scalarString.assign((char*)event.data.scalar.value, event.data.scalar.length);
                if (nodeStack.empty())
                    retVal = std::move(newScalar);
                else
                    InsertNode(nodeStack, mapKey, retVal, std::move(newScalar));
                break;
            }
            case YAML_SEQUENCE_START_EVENT:
            {
                YAMLNode<STLTRAITS>* newSeq = new YAMLNode<STLTRAITS>(YAML_SEQUENCE_NODE);
                InsertNode(nodeStack, mapKey, retVal, std::unique_ptr<YAMLNode<STLTRAITS>>(newSeq));
                nodeStack.emplace_back(newSeq);
                break;
            }
            case YAML_SEQUENCE_END_EVENT:
            {
                nodeStack.pop_back();
                break;
            }
            case YAML_MAPPING_START_EVENT:
            {
                YAMLNode<STLTRAITS>* newMap = new YAMLNode<STLTRAITS>(YAML_MAPPING_NODE);
                InsertNode(nodeStack, mapKey, retVal, std::unique_ptr<YAMLNode<STLTRAITS>>(newMap));
                nodeStack.emplace_back(newMap);
                break;
            }
            case YAML_MAPPING_END_EVENT:
            {
                nodeStack.pop_back();
                break;
            }
            case YAML_DOCUMENT_END_EVENT:
            {
                yaml_event_delete(&event);
                return retVal;
            }
            default:
                break;
            }
            yaml_event_delete(&event);
        }
        return std::unique_ptr<YAMLNode<STLTRAITS>>();
    }
public:
    using StlTraits = STLTRAITS;

    inline const YAMLNode<STLTRAITS>* getRootNode() const {return m_rootNode.get();}
    bool read(yaml_parser_t* doc)
    {
        std::unique_ptr<YAMLNode<STLTRAITS>> newRoot = ParseEvents(doc);
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
        YAMLNode<STLTRAITS>* curSub = m_subStack.back();
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
        YAMLNode<STLTRAITS>* curSub = m_subStack.back();
        for (const auto& item : curSub->m_mapChildren)
        {
            if (!item.first.compare(name))
            {
                YAMLNode<STLTRAITS>* nextSub = item.second.get();
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
    void enumerate(const char* name, typename STLTRAITS::template Vector<T>& vector, size_t count,
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
    void enumerate(const char* name, typename STLTRAITS::template Vector<T>& vector, size_t count,
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
    void enumerate(const char* name, typename STLTRAITS::template Vector<T>& vector, size_t count,
                   std::function<void(YAMLDocReader<STLTRAITS>&, T&)> readf)
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
            const YAMLNode<STLTRAITS>* mnode = m_subStack.back();
            if (mnode->m_type == YAML_SCALAR_NODE)
            {
                return YAMLTranslate<STLTRAITS>::template NodeToVal<RETURNTYPE>(mnode);
            }
            else if (mnode->m_type == YAML_SEQUENCE_NODE)
            {
                int& seqIdx = m_seqTrackerStack.back();
                return YAMLTranslate<STLTRAITS>::template NodeToVal<RETURNTYPE>(mnode->m_seqChildren[seqIdx++].get());
            }
            else if (mnode->m_type == YAML_MAPPING_NODE)
            {
                for (const auto& item : mnode->m_mapChildren)
                {
                    if (!item.first.compare(name))
                    {
                        return YAMLTranslate<STLTRAITS>::template NodeToVal<RETURNTYPE>(item.second.get());
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

    inline typename STLTRAITS::String readString(const char* name)
    {
        return readVal<typename STLTRAITS::String>(name);
    }

    inline typename STLTRAITS::WString readWString(const char* name)
    {
        return readVal<typename STLTRAITS::WString>(name);
    }

};

template <typename STLTRAITS>
class YAMLDocWriter
{
    YAMLNode<STLTRAITS> m_rootNode;
    typename STLTRAITS::template Vector<YAMLNode<STLTRAITS>*> m_subStack;
    static bool RecursiveFinish(yaml_emitter_t* doc, const YAMLNode<STLTRAITS>& node)
    {
        yaml_event_t event;
        if (node.m_type == YAML_SCALAR_NODE)
        {
            if (!yaml_scalar_event_initialize(&event, nullptr, nullptr, (yaml_char_t*)node.m_scalarString.c_str(),
                                              node.m_scalarString.length(), true, true, ScalarStyle(node)) ||
                !yaml_emitter_emit(doc, &event))
                goto err;
        }
        else if (node.m_type == YAML_SEQUENCE_NODE)
        {
            if (!yaml_sequence_start_event_initialize(&event, nullptr, nullptr, 1, SequenceStyle(node)) ||
                !yaml_emitter_emit(doc, &event))
                goto err;
            for (const auto& item : node.m_seqChildren)
            {
                if (!RecursiveFinish(doc, *item))
                    goto err;
            }
            if (!yaml_sequence_end_event_initialize(&event) ||
                !yaml_emitter_emit(doc, &event))
                goto err;
        }
        else if (node.m_type == YAML_MAPPING_NODE)
        {
            if (!yaml_mapping_start_event_initialize(&event, nullptr, nullptr, true, MappingStyle(node)) ||
                !yaml_emitter_emit(doc, &event))
                goto err;
            for (const auto& item : node.m_mapChildren)
            {
                if (!EmitKeyScalar(doc, item.first.c_str()))
                    goto err;
                if (!RecursiveFinish(doc, *item.second))
                    goto err;
            }
            event.type = YAML_MAPPING_END_EVENT;
            if (!yaml_mapping_end_event_initialize(&event) ||
                !yaml_emitter_emit(doc, &event))
                goto err;

        }
        return true;
    err:
        HandleYAMLEmitterError(doc);
        return false;
    }
public:
    using StlTraits = STLTRAITS;

    YAMLDocWriter() : m_rootNode(YAML_MAPPING_NODE)
    {
        m_subStack.emplace_back(&m_rootNode);
    }

    void enterSubRecord(const char* name)
    {
        YAMLNode<STLTRAITS>* curSub = m_subStack.back();
        if (curSub->m_type != YAML_MAPPING_NODE &&
            curSub->m_type != YAML_SEQUENCE_NODE)
            return;
        YAMLNode<STLTRAITS>* newNode = new YAMLNode<STLTRAITS>(YAML_MAPPING_NODE);
        if (curSub->m_type == YAML_MAPPING_NODE)
            curSub->m_mapChildren.emplace_back(name?typename STLTRAITS::String(name):typename STLTRAITS::String(),
                                               std::unique_ptr<YAMLNode<STLTRAITS>>(newNode));
        else if (curSub->m_type == YAML_SEQUENCE_NODE)
            curSub->m_seqChildren.emplace_back(newNode);
        m_subStack.push_back(newNode);
    }

    void leaveSubRecord()
    {
        if (m_subStack.size() > 1)
        {
            YAMLNode<STLTRAITS>* curSub = m_subStack.back();
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
        YAMLNode<STLTRAITS>* curSub = m_subStack.back();
        if (curSub->m_type != YAML_MAPPING_NODE &&
            curSub->m_type != YAML_SEQUENCE_NODE)
            return;
        YAMLNode<STLTRAITS>* newNode = new YAMLNode<STLTRAITS>(YAML_SEQUENCE_NODE);
        if (curSub->m_type == YAML_MAPPING_NODE)
            curSub->m_mapChildren.emplace_back(name?typename STLTRAITS::String(name):typename STLTRAITS::String(),
                                               std::unique_ptr<YAMLNode<STLTRAITS>>(newNode));
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
    void enumerate(const char* name, const typename STLTRAITS::template Vector<T>& vector,
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
    void enumerate(const char* name, const typename STLTRAITS::template Vector<T>& vector,
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
    void enumerate(const char* name, const typename STLTRAITS::template Vector<T>& vector,
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
        YAMLNode<STLTRAITS>* curSub = m_subStack.back();
        if (curSub->m_type == YAML_MAPPING_NODE)
            curSub->m_mapChildren.emplace_back(name?name:typename STLTRAITS::String(),
                                               std::move(YAMLTranslate<StlTraits>::ValToNode(val)));
        else if (curSub->m_type == YAML_SEQUENCE_NODE)
            curSub->m_seqChildren.emplace_back(std::move(YAMLTranslate<StlTraits>::ValToNode(val)));
    }

    template <typename INTYPE>
    void writeVal(const char* name, INTYPE val, size_t byteCount)
    {
        YAMLNode<STLTRAITS>* curSub = m_subStack.back();
        if (curSub->m_type == YAML_MAPPING_NODE)
            curSub->m_mapChildren.emplace_back(name?name:typename STLTRAITS::String(),
                                               std::move(YAMLTranslate<StlTraits>::ValToNode(val, byteCount)));
        else if (curSub->m_type == YAML_SEQUENCE_NODE)
            curSub->m_seqChildren.emplace_back(std::move(YAMLTranslate<StlTraits>::ValToNode(val, byteCount)));
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

    inline void writeString(const char* name, const typename STLTRAITS::String& val)
    {
        writeVal<typename STLTRAITS::String>(name, val);
    }

    inline void writeString(const char* name, const char* val)
    {
        writeVal<const char*>(name, val);
    }

    inline void writeWString(const char* name, const typename STLTRAITS::WString& val)
    {
        writeVal<typename STLTRAITS::WString>(name, val);
    }

    inline void writeWString(const char* name, const wchar_t* val)
    {
        writeVal<const wchar_t*>(name, val);
    }
};

/* forward-declaration dance for recursively-derived types */

template <size_t sizeVar, Endian VE, class STLTRAITS>
struct BufferYaml;

template <atInt32 sizeVar, Endian VE, class STLTRAITS>
struct StringYaml;

template <atInt32 sizeVar, Endian VE, class STLTRAITS>
struct WStringYaml;

template <atInt32 sizeVar, Endian VE, class STLTRAITS>
struct WStringAsStringYaml;

template <Endian DNAE, typename STLTRAITS = StlTraits>
struct DNAYaml : DNA<DNAE, STLTRAITS>
{
    using YAMLDocReader = YAMLDocReader<STLTRAITS>;
    using YAMLDocWriter = YAMLDocWriter<STLTRAITS>;
    using YAMLNode = YAMLNode<STLTRAITS>;
    using YAMLTranslate = YAMLTranslate<STLTRAITS>;

    virtual void toYAML(YAMLDocWriter& out) const=0;
    virtual void fromYAML(YAMLDocReader& in)=0;

    template <size_t sizeVar>
    using Buffer = struct Athena::io::BufferYaml<sizeVar, DNAE, STLTRAITS>;

    template <atInt32 sizeVar = -1>
    using String = struct Athena::io::StringYaml<sizeVar, DNAE, STLTRAITS>;

    template <atInt32 sizeVar = -1, Endian VE = DNAE>
    using WString = struct Athena::io::WStringYaml<sizeVar, VE, STLTRAITS>;

    template <atInt32 sizeVar = -1>
    using WStringAsString = struct Athena::io::WStringAsStringYaml<sizeVar, DNAE, STLTRAITS>;

    typename STLTRAITS::String toYAMLString() const
    {
        yaml_emitter_t emitter;
        if (!yaml_emitter_initialize(&emitter))
        {
            HandleYAMLEmitterError(&emitter);
            return typename STLTRAITS::String();
        }

        typename STLTRAITS::String res;
        yaml_emitter_set_output(&emitter, (yaml_write_handler_t*)YAMLStdStringWriter<STLTRAITS>, &res);
        yaml_emitter_set_unicode(&emitter, true);
        yaml_emitter_set_width(&emitter, -1);

        if (!yaml_emitter_open(&emitter))
        {
            HandleYAMLEmitterError(&emitter);
            yaml_emitter_delete(&emitter);
            return typename STLTRAITS::String();
        }
        {
            YAMLDocWriter docWriter;
            toYAML(docWriter);
            if (!docWriter.finish(&emitter))
            {
                yaml_emitter_delete(&emitter);
                return typename STLTRAITS::String();
            }
        }
        if (!yaml_emitter_close(&emitter) ||
            !yaml_emitter_flush(&emitter))
        {
            HandleYAMLEmitterError(&emitter);
            yaml_emitter_delete(&emitter);
            return typename STLTRAITS::String();
        }
        yaml_emitter_delete(&emitter);

        return res;
    }

    bool fromYAMLString(const typename STLTRAITS::String& str)
    {
        yaml_parser_t parser;
        if (!yaml_parser_initialize(&parser))
        {
            HandleYAMLParserError(&parser);
            return false;
        }

        YAMLStdStringReaderState<STLTRAITS> reader(str);
        yaml_parser_set_input(&parser, (yaml_read_handler_t*)YAMLStdStringReader<STLTRAITS>, &reader);

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

template <size_t sizeVar, Endian VE, class STLTRAITS>
struct BufferYaml : public DNAYaml<VE, STLTRAITS>, public std::unique_ptr<atUint8[]>
{
    typename DNA<VE>::Delete expl;
    inline void read(typename DNAYaml<VE, STLTRAITS>::IStreamReader& reader)
    {
        reset(new atUint8[sizeVar]);
        reader.readUBytesToBuf(get(), sizeVar);
    }
    inline void write(typename DNAYaml<VE, STLTRAITS>::IStreamWriter& writer) const
    {
        writer.writeUBytes(get(), sizeVar);
    }
    inline void fromYAML(YAMLDocReader<STLTRAITS>& reader)
    {*this = reader.readUBytes(nullptr);}
    inline void toYAML(YAMLDocWriter<STLTRAITS>& writer) const
    {writer.writeUBytes(nullptr, *this, sizeVar);}
};

template <atInt32 sizeVar, Endian VE, class STLTRAITS>
struct StringYaml : public DNAYaml<VE, STLTRAITS>, public STLTRAITS::String
{
    typename DNA<VE>::Delete expl;
    inline void read(typename DNAYaml<VE, STLTRAITS>::IStreamReader& reader)
    {this->assign(std::move(reader.readString(sizeVar)));}
    inline void write(typename DNAYaml<VE, STLTRAITS>::IStreamWriter& writer) const
    {writer.writeString(*this, sizeVar);}
    inline void fromYAML(YAMLDocReader<STLTRAITS>& reader)
    {this->assign(std::move(reader.readString(nullptr)));}
    inline void toYAML(YAMLDocWriter<STLTRAITS>& writer) const
    {writer.writeString(nullptr, *this);}
    inline typename STLTRAITS::String& operator=(const typename STLTRAITS::String& __str)
    {return this->assign(__str);}
    inline typename STLTRAITS::String& operator=(typename STLTRAITS::String&& __str)
    {this->swap(__str); return *this;}
};

template <atInt32 sizeVar, Endian VE, class STLTRAITS>
struct WStringYaml : public DNAYaml<VE, STLTRAITS>, public STLTRAITS::WString
{
    typename DNA<VE>::Delete expl;
    inline void read(typename DNAYaml<VE, STLTRAITS>::IStreamReader& reader)
    {
        reader.setEndian(VE);
        this->assign(std::move(reader.readWString(sizeVar)));
    }
    inline void write(typename DNAYaml<VE, STLTRAITS>::IStreamWriter& writer) const
    {
        writer.setEndian(VE);
        writer.writeWString(*this, sizeVar);
    }
    inline void fromYAML(YAMLDocReader<STLTRAITS>& reader)
    {this->assign(std::move(reader.readWString(nullptr)));}
    inline void toYAML(YAMLDocWriter<STLTRAITS>& writer) const
    {writer.writeWString(nullptr, *this);}
    inline typename STLTRAITS::WSTRING& operator=(const typename STLTRAITS::WSTRING& __str)
    {return this->assign(__str);}
    inline typename STLTRAITS::WSTRING& operator=(typename STLTRAITS::WSTRING&& __str)
    {this->swap(__str); return *this;}
};

template <atInt32 sizeVar, Endian VE, class STLTRAITS>
struct WStringAsStringYaml : public DNAYaml<VE, STLTRAITS>, public STLTRAITS::String
{
    typename DNA<VE>::Delete expl;
    inline void read(typename DNAYaml<VE, STLTRAITS>::IStreamReader& reader)
    {*this = reader.readWStringAsString(sizeVar);}
    inline void write(typename DNAYaml<VE, STLTRAITS>::IStreamWriter& writer) const
    {writer.writeStringAsWString(*this, sizeVar);}
    inline void fromYAML(YAMLDocReader<STLTRAITS>& reader)
    {this->assign(std::move(reader.readString(nullptr)));}
    inline void toYAML(YAMLDocWriter<STLTRAITS>& writer) const
    {writer.writeString(nullptr, *this);}
    inline typename STLTRAITS::String& operator=(const typename STLTRAITS::String& __str)
    {return this->assign(__str);}
    inline typename STLTRAITS::String& operator=(typename STLTRAITS::String&& __str)
    {this->swap(__str); return *this;}
};

/** Macro to automatically declare YAML read/write methods in subclasses */
#define DECL_YAML \
    DECL_DNA \
    void fromYAML(YAMLDocReader&); \
    void toYAML(YAMLDocWriter&) const; \

/** Macro to automatically declare YAML read/write methods with client-code's definition */
#define DECL_EXPLICIT_YAML \
    void fromYAML(YAMLDocReader&); \
    void toYAML(YAMLDocWriter&) const; \

}
}

#endif // DNAYAML_HPP
