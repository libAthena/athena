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

const char* AT_BASE64_CHARS =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";



}
}
