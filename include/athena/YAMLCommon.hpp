//
// Created by Jack Andersen on 2/16/18.
//

#ifndef URDE_YAMLCOMMON_HPP
#define URDE_YAMLCOMMON_HPP

#include <cstring>
#include <yaml.h>
#include <utf8proc.h>
#include <vector>
#include "Global.hpp"

namespace athena::io
{
class IStreamReader;
class IStreamWriter;

enum class YAMLNodeStyle
{
    Any,
    Flow,
    Block
};

struct YAMLNode
{
    yaml_node_type_t m_type;
    std::string m_scalarString;
    std::vector<std::unique_ptr<YAMLNode>> m_seqChildren;
    std::vector<std::pair<std::string, std::unique_ptr < YAMLNode>>>
    m_mapChildren;
    YAMLNodeStyle m_style = YAMLNodeStyle::Any;

    YAMLNode(yaml_node_type_t type) : m_type(type) {}

    inline const YAMLNode* findMapChild(std::string_view key) const
    {
        for (const auto& item : m_mapChildren)
            if (!item.first.compare(key))
                return item.second.get();
        return nullptr;
    }

    inline void assignMapChild(std::string_view key, std::unique_ptr <YAMLNode>&& node)
    {
        for (auto& item : m_mapChildren)
            if (!item.first.compare(key))
            {
                item.second = std::move(node);
                return;
            }
        m_mapChildren.emplace_back(key, std::move(node));
    }
};

template<typename RETURNTYPE>
RETURNTYPE NodeToVal(const YAMLNode* node);

template<>
bool NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(bool val);

template<>
atInt8 NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(atInt8 val);

template<>
atUint8 NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(atUint8 val);

template<>
atInt16 NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(atInt16 val);

template<>
atUint16 NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(atUint16 val);

template<>
atInt32 NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(atInt32 val);

template<>
atUint32 NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(atUint32 val);

template<>
atInt64 NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(atInt64 val);

template<>
atUint64 NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(atUint64 val);

template<>
float NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(float val);

template<>
double NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(double val);

template<typename RETURNTYPE>
RETURNTYPE NodeToVec(const YAMLNode* node);

template<>
atVec2f NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(const atVec2f& val);

template<>
atVec3f NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(const atVec3f& val);

template<>
atVec4f NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(const atVec4f& val);

template<>
atVec2d NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(const atVec2d& val);

template<>
atVec3d NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(const atVec3d& val);

template<>
atVec4d NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(const atVec4d& val);

template<>
std::unique_ptr<atUint8[]> NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(const std::unique_ptr<atUint8[]>& val, size_t byteCount);

template<>
std::string NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(std::string_view val);

template<>
std::wstring NodeToVal(const YAMLNode* node);

std::unique_ptr <YAMLNode> ValToNode(std::wstring_view val);

std::unique_ptr <YAMLNode> ValToNode(std::u16string_view val);

std::unique_ptr <YAMLNode> ValToNode(std::u32string_view val);

std::string base64_encode(const atUint8* bytes_to_encode, size_t in_len);

std::unique_ptr<atUint8[]> base64_decode(std::string_view encoded_string);

void HandleYAMLParserError(yaml_parser_t* parser);

void HandleYAMLEmitterError(yaml_emitter_t* emitter);

struct YAMLStdStringViewReaderState
{
    std::string_view::const_iterator begin;
    std::string_view::const_iterator end;

    YAMLStdStringViewReaderState(std::string_view str)
    {
        begin = str.begin();
        end = str.end();
    }
};

int YAMLStdStringReader(YAMLStdStringViewReaderState* str,
                        unsigned char* buffer, size_t size, size_t* size_read);

int YAMLStdStringWriter(std::string* str, unsigned char* buffer, size_t size);

int YAMLAthenaReader(athena::io::IStreamReader* reader,
                     unsigned char* buffer, size_t size, size_t* size_read);

int YAMLAthenaWriter(athena::io::IStreamWriter* writer,
                     unsigned char* buffer, size_t size);

}

#endif //URDE_YAMLCOMMON_HPP
