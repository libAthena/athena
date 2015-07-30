#include "Athena/DNAYaml.hpp"

namespace Athena
{
namespace io
{

static const char* ErrorString(yaml_error_type_t errt)
{
    switch (errt)
    {
    case YAML_NO_ERROR:
        return "No Error";
    case YAML_MEMORY_ERROR:
        return "Memory Error";
    case YAML_READER_ERROR:
        return "Reader Error";
    case YAML_SCANNER_ERROR:
        return "Scanner Error";
    case YAML_PARSER_ERROR:
        return "Parser Error";
    case YAML_COMPOSER_ERROR:
        return "Composer Error";
    case YAML_WRITER_ERROR:
        return "Writer Error";
    case YAML_EMITTER_ERROR:
        return "Emitter Error";
    }
    return "Unknown Error";
}

void HandleYAMLParserError(yaml_parser_t* parser)
{
    atError("YAML error: %s: %s", ErrorString(parser->error), parser->problem?parser->problem:"");
}

void HandleYAMLEmitterError(yaml_emitter_t* emitter)
{
    atError("YAML error: %s: %s", ErrorString(emitter->error), emitter->problem?emitter->problem:"");
}

int YAMLStdStringReader(YAMLStdStringReaderState* reader,
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

int YAMLStdStringWriter(std::string* str, unsigned char *buffer, size_t size)
{
    str->append((char*)buffer, size);
    return 1;
}

static inline void InsertNode(std::vector<YAMLNode*>& nodeStack,
                              std::unique_ptr<YAMLNode>& mapKey,
                              std::unique_ptr<YAMLNode>& retVal,
                              std::unique_ptr<YAMLNode>&& newNode)
{
    if (nodeStack.empty())
    {
        retVal = std::move(newNode);
        return;
    }
    YAMLNode* parent = nodeStack.back();
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

std::unique_ptr<YAMLNode> YAMLDocReader::ParseEvents(yaml_parser_t* doc)
{
    yaml_event_t event;
    if (!yaml_parser_parse(doc, &event))
    {
        HandleYAMLParserError(doc);
        return std::unique_ptr<YAMLNode>();
    }

    std::vector<YAMLNode*> nodeStack;
    std::unique_ptr<YAMLNode> mapKey;
    std::unique_ptr<YAMLNode> retVal;
    int result;
    for (result = yaml_parser_parse(doc, &event);
         event.type != YAML_STREAM_END_EVENT;
         result = yaml_parser_parse(doc, &event))
    {
        if (!result)
        {
            HandleYAMLParserError(doc);
            return std::unique_ptr<YAMLNode>();
        }
        switch (event.type)
        {
        case YAML_SCALAR_EVENT:
        {
            if (nodeStack.empty())
            {
                atWarning("YAML parser stack empty; skipping scalar node");
                yaml_event_delete(&event);
                continue;
            }
            std::unique_ptr<YAMLNode> newScalar(new YAMLNode(YAML_SCALAR_NODE));
            newScalar->m_scalarString.assign((char*)event.data.scalar.value, event.data.scalar.length);
            if (nodeStack.empty())
                retVal = std::move(newScalar);
            else
                InsertNode(nodeStack, mapKey, retVal, std::move(newScalar));
            break;
        }
        case YAML_SEQUENCE_START_EVENT:
        {
            YAMLNode* newSeq = new YAMLNode(YAML_SEQUENCE_NODE);
            InsertNode(nodeStack, mapKey, retVal, std::unique_ptr<YAMLNode>(newSeq));
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
            YAMLNode* newMap = new YAMLNode(YAML_MAPPING_NODE);
            InsertNode(nodeStack, mapKey, retVal, std::unique_ptr<YAMLNode>(newMap));
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
    return std::unique_ptr<YAMLNode>();
}

static inline bool EmitKeyScalar(yaml_emitter_t* doc, const char* val)
{
    yaml_event_t event;
    if (!yaml_scalar_event_initialize(&event, nullptr, nullptr, (yaml_char_t*)val,
                                      strlen(val), true, true, YAML_PLAIN_SCALAR_STYLE))
        return false;
    return yaml_emitter_emit(doc, &event);
}

static inline yaml_scalar_style_t ScalarStyle(const YAMLNode& node)
{
    for (const auto& ch : node.m_scalarString)
        if (ch == '\n')
            return YAML_LITERAL_SCALAR_STYLE;
    return YAML_ANY_SCALAR_STYLE;
}

static inline yaml_sequence_style_t SequenceStyle(const YAMLNode& node)
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

static inline yaml_mapping_style_t MappingStyle(const YAMLNode& node)
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

bool YAMLDocWriter::RecursiveFinish(yaml_emitter_t* doc, const YAMLNode& node)
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
            if (!RecursiveFinish(doc, *item.get()))
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
            if (!RecursiveFinish(doc, *item.second.get()))
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

static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";


static inline bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(const atUint8* bytes_to_encode, size_t in_len)
{
    std::string ret;
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
                ret += base64_chars[char_array_4[i]];
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
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';

    }

    return ret;

}
std::unique_ptr<atUint8[]> base64_decode(const std::string& encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::unique_ptr<atUint8[]> ret(new atUint8[in_len * 3 / 4]);
    atUint8* retBuf = ret.get();

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

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
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) *retBuf++ = char_array_3[j];
    }

    return ret;
}

}
}
