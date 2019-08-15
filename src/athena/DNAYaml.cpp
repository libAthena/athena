#include "athena/DNAYaml.hpp"

namespace athena::io {

template <>
bool NodeToVal(const YAMLNode* node) {
  char firstCh = tolower(node->m_scalarString[0]);
  if (firstCh == 't')
    return true;
  else if (firstCh == 'f')
    return false;
  else if (isdigit(firstCh) && firstCh != 0)
    return true;
  return false;
}

std::unique_ptr<YAMLNode> ValToNode(bool val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = val ? "True" : "False";
  return ret;
}

template <>
atInt8 NodeToVal(const YAMLNode* node) {
  return strtol(node->m_scalarString.c_str(), nullptr, 0);
}

std::unique_ptr<YAMLNode> ValToNode(atInt8 val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = fmt::format(fmt("{}"), int(val));
  return ret;
}

template <>
atUint8 NodeToVal(const YAMLNode* node) {
  return strtoul(node->m_scalarString.c_str(), nullptr, 0);
}

std::unique_ptr<YAMLNode> ValToNode(atUint8 val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = fmt::format(fmt("0x{:02X}"), val);
  return ret;
}

template <>
atInt16 NodeToVal(const YAMLNode* node) {
  return strtol(node->m_scalarString.c_str(), nullptr, 0);
}

std::unique_ptr<YAMLNode> ValToNode(atInt16 val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = fmt::format(fmt("{}"), int(val));
  return ret;
}

template <>
atUint16 NodeToVal(const YAMLNode* node) {
  return strtoul(node->m_scalarString.c_str(), nullptr, 0);
}

std::unique_ptr<YAMLNode> ValToNode(atUint16 val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = fmt::format(fmt("0x{:04X}"), val);
  return ret;
}

template <>
atInt32 NodeToVal(const YAMLNode* node) {
  return strtol(node->m_scalarString.c_str(), nullptr, 0);
}

std::unique_ptr<YAMLNode> ValToNode(atInt32 val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = fmt::format(fmt("{}"), int(val));
  return ret;
}

template <>
atUint32 NodeToVal(const YAMLNode* node) {
  return strtoul(node->m_scalarString.c_str(), nullptr, 0);
}

std::unique_ptr<YAMLNode> ValToNode(atUint32 val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = fmt::format(fmt("0x{:08X}"), val);
  return ret;
}

template <>
atInt64 NodeToVal(const YAMLNode* node) {
  return std::strtoll(node->m_scalarString.c_str(), nullptr, 0);
}

std::unique_ptr<YAMLNode> ValToNode(atInt64 val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = fmt::format(fmt("{}"), val);
  return ret;
}

template <>
atUint64 NodeToVal(const YAMLNode* node) {
  return std::strtoull(node->m_scalarString.c_str(), nullptr, 0);
}

std::unique_ptr<YAMLNode> ValToNode(atUint64 val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = fmt::format(fmt("0x{:016X}"), val);
  return ret;
}

template <>
float NodeToVal(const YAMLNode* node) {
  return strtof(node->m_scalarString.c_str(), nullptr);
}

std::unique_ptr<YAMLNode> ValToNode(float val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = fmt::format(fmt("{}"), val);
  return ret;
}

template <>
double NodeToVal(const YAMLNode* node) {
  return strtod(node->m_scalarString.c_str(), nullptr);
}

std::unique_ptr<YAMLNode> ValToNode(double val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = fmt::format(fmt("{}"), val);
  return ret;
}

template <typename RETURNTYPE>
RETURNTYPE NodeToVec(const YAMLNode* node) {
  constexpr bool isDouble = std::is_same<RETURNTYPE, atVec2d>::value || std::is_same<RETURNTYPE, atVec3d>::value ||
                            std::is_same<RETURNTYPE, atVec4d>::value;
  RETURNTYPE retval = {};
  auto it = node->m_seqChildren.begin();
  simd_values<std::conditional_t<isDouble, double, float>> f;
  for (size_t i = 0; i < 4 && it != node->m_seqChildren.end(); ++i, ++it) {
    YAMLNode* snode = it->get();
    if (snode->m_type == YAML_SCALAR_NODE) {
      if (isDouble)
        f[i] = NodeToVal<double>(snode);
      else
        f[i] = NodeToVal<float>(snode);
    } else
      f[i] = 0.0;
  }
  retval.simd.copy_from(f);
  return retval;
}

template <>
atVec2f NodeToVal(const YAMLNode* node) {
  return NodeToVec<atVec2f>(node);
}

std::unique_ptr<YAMLNode> ValToNode(const atVec2f& val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SEQUENCE_NODE);
  ret->m_seqChildren.reserve(2);
  simd_floats f(val.simd);
  for (size_t i = 0; i < 2; ++i) {
    auto comp = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
    comp->m_scalarString = fmt::format(fmt("{}"), f[i]);
    ret->m_seqChildren.emplace_back(std::move(comp));
  }
  return ret;
}

template <>
atVec3f NodeToVal(const YAMLNode* node) {
  return NodeToVec<atVec3f>(node);
}

std::unique_ptr<YAMLNode> ValToNode(const atVec3f& val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SEQUENCE_NODE);
  ret->m_seqChildren.reserve(3);
  simd_floats f(val.simd);
  for (size_t i = 0; i < 3; ++i) {
    auto comp = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
    comp->m_scalarString = fmt::format(fmt("{}"), f[i]);
    ret->m_seqChildren.emplace_back(std::move(comp));
  }
  return ret;
}

template <>
atVec4f NodeToVal(const YAMLNode* node) {
  return NodeToVec<atVec4f>(node);
}

std::unique_ptr<YAMLNode> ValToNode(const atVec4f& val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SEQUENCE_NODE);
  ret->m_seqChildren.reserve(4);
  simd_floats f(val.simd);
  for (size_t i = 0; i < 4; ++i) {
    auto comp = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
    comp->m_scalarString = fmt::format(fmt("{}"), f[i]);
    ret->m_seqChildren.emplace_back(std::move(comp));
  }
  return ret;
}

template <>
atVec2d NodeToVal(const YAMLNode* node) {
  return NodeToVec<atVec2d>(node);
}

std::unique_ptr<YAMLNode> ValToNode(const atVec2d& val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SEQUENCE_NODE);
  ret->m_seqChildren.reserve(2);
  simd_doubles f(val.simd);
  for (size_t i = 0; i < 2; ++i) {
    auto comp = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
    comp->m_scalarString = fmt::format(fmt("{}"), f[i]);
    ret->m_seqChildren.emplace_back(std::move(comp));
  }
  return ret;
}

template <>
atVec3d NodeToVal(const YAMLNode* node) {
  return NodeToVec<atVec3d>(node);
}

std::unique_ptr<YAMLNode> ValToNode(const atVec3d& val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SEQUENCE_NODE);
  ret->m_seqChildren.reserve(3);
  simd_doubles f(val.simd);
  for (size_t i = 0; i < 3; ++i) {
    auto comp = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
    comp->m_scalarString = fmt::format(fmt("{}"), f[i]);
    ret->m_seqChildren.emplace_back(std::move(comp));
  }
  return ret;
}

template <>
atVec4d NodeToVal(const YAMLNode* node) {
  return NodeToVec<atVec4d>(node);
}

std::unique_ptr<YAMLNode> ValToNode(const atVec4d& val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SEQUENCE_NODE);
  ret->m_seqChildren.reserve(4);
  simd_doubles f(val.simd);
  for (size_t i = 0; i < 4; ++i) {
    auto comp = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
    comp->m_scalarString = fmt::format(fmt("{}"), f[i]);
    ret->m_seqChildren.emplace_back(std::move(comp));
  }
  return ret;
}

template <>
std::unique_ptr<atUint8[]> NodeToVal(const YAMLNode* node) {
  return base64_decode(node->m_scalarString);
}

std::unique_ptr<YAMLNode> ValToNode(const std::unique_ptr<atUint8[]>& val, size_t byteCount) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  if (val)
    ret->m_scalarString = base64_encode(val.get(), byteCount);
  return ret;
}

template <>
std::string NodeToVal(const YAMLNode* node) {
  return node->m_scalarString;
}

std::unique_ptr<YAMLNode> ValToNode(std::string_view val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString = val;
  return ret;
}

template <>
std::wstring NodeToVal(const YAMLNode* node) {
  std::wstring retval;
  retval.reserve(node->m_scalarString.length());
  const utf8proc_uint8_t* buf = reinterpret_cast<const utf8proc_uint8_t*>(node->m_scalarString.c_str());
  while (*buf) {
    utf8proc_int32_t wc;
    utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
    if (len < 0) {
      atWarning(fmt("invalid UTF-8 character while decoding"));
      return retval;
    }
    buf += len;
    retval += wchar_t(wc);
  }
  return retval;
}

std::unique_ptr<YAMLNode> ValToNode(std::wstring_view val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString.reserve(val.length());
  for (wchar_t ch : val) {
    utf8proc_uint8_t mb[4];
    utf8proc_ssize_t c = utf8proc_encode_char(utf8proc_int32_t(ch), mb);
    if (c < 0) {
      atWarning(fmt("invalid UTF-8 character while encoding"));
      return ret;
    }
    ret->m_scalarString.append(reinterpret_cast<char*>(mb), c);
  }
  return ret;
}

std::unique_ptr<YAMLNode> ValToNode(std::u16string_view val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString.reserve(val.length());
  for (char16_t ch : val) {
    utf8proc_uint8_t mb[4];
    utf8proc_ssize_t c = utf8proc_encode_char(utf8proc_int32_t(ch), mb);
    if (c < 0) {
      atWarning(fmt("invalid UTF-8 character while encoding"));
      return ret;
    }
    ret->m_scalarString.append(reinterpret_cast<char*>(mb), c);
  }
  return ret;
}

std::unique_ptr<YAMLNode> ValToNode(std::u32string_view val) {
  auto ret = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
  ret->m_scalarString.reserve(val.length());
  for (char32_t ch : val) {
    utf8proc_uint8_t mb[4];
    utf8proc_ssize_t c = utf8proc_encode_char(utf8proc_int32_t(ch), mb);
    if (c < 0) {
      atWarning(fmt("invalid UTF-8 character while encoding"));
      return ret;
    }
    ret->m_scalarString.append(reinterpret_cast<char*>(mb), c);
  }
  return ret;
}

static const char* ErrorString(yaml_error_type_t errt) {
  switch (errt) {
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

void HandleYAMLParserError(yaml_parser_t* parser) {
  atError(fmt("YAML error: {}: {}"), ErrorString(parser->error), parser->problem ? parser->problem : "");
}

void HandleYAMLEmitterError(yaml_emitter_t* emitter) {
  atError(fmt("YAML error: {}: {}"), ErrorString(emitter->error), emitter->problem ? emitter->problem : "");
}

int YAMLStdStringReader(YAMLStdStringViewReaderState* reader, unsigned char* buffer, size_t size, size_t* size_read) {
  size_t diff = reader->end - reader->begin;
  if (!diff) {
    *size_read = 0;
  } else if (diff < size) {
    memcpy(buffer, &*reader->begin, diff);
    *size_read = diff;
  } else {
    memcpy(buffer, &*reader->begin, size);
    *size_read = size;
  }
  return 1;
}

int YAMLStdStringWriter(std::string* str, unsigned char* buffer, size_t size) {
  str->append((char*)buffer, size);
  return 1;
}

int YAMLAthenaReader(athena::io::IStreamReader* reader, unsigned char* buffer, size_t size, size_t* size_read) {
  *size_read = reader->readUBytesToBuf(buffer, size);
  return 1;
}

int YAMLAthenaWriter(athena::io::IStreamWriter* writer, unsigned char* buffer, size_t size) {
  writer->writeUBytes(buffer, size);
  return 1;
}

YAMLDocWriter::YAMLDocWriter(const char* classType, athena::io::IStreamReader* reader) {
  if (!yaml_emitter_initialize(&m_emitter)) {
    HandleYAMLEmitterError(&m_emitter);
    return;
  }
  yaml_emitter_set_unicode(&m_emitter, true);
  yaml_emitter_set_width(&m_emitter, -1);

  if (reader) {
    YAMLDocReader r;
    r.parse(reader);
    m_rootNode = r.releaseRootNode();
    if (!m_rootNode)
      m_rootNode = std::make_unique<YAMLNode>(YAML_MAPPING_NODE);
  } else {
    m_rootNode = std::make_unique<YAMLNode>(YAML_MAPPING_NODE);
  }

  m_subStack.emplace_back(m_rootNode.get());
  if (classType) {
    auto classVal = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
    classVal->m_scalarString.assign(classType);
    m_rootNode->assignMapChild("DNAType", std::move(classVal));
  }
}

YAMLDocWriter::~YAMLDocWriter() { yaml_emitter_delete(&m_emitter); }

bool YAMLDocWriter::finish(athena::io::IStreamWriter* fout) {
  yaml_event_t event = {};

  if (fout)
    yaml_emitter_set_output(&m_emitter, (yaml_write_handler_t*)YAMLAthenaWriter, fout);
  if (!yaml_emitter_open(&m_emitter))
    goto err;

  event.type = YAML_DOCUMENT_START_EVENT;
  event.data.document_start.implicit = true;
  if (!yaml_emitter_emit(&m_emitter, &event))
    goto err;
  if (!RecursiveFinish(&m_emitter, *m_rootNode))
    return false;
  event.type = YAML_DOCUMENT_END_EVENT;
  event.data.document_end.implicit = true;
  if (!yaml_emitter_emit(&m_emitter, &event))
    goto err;

  if (!yaml_emitter_close(&m_emitter) || !yaml_emitter_flush(&m_emitter))
    goto err;

  return true;
err:
  HandleYAMLEmitterError(&m_emitter);
  return false;
}

YAMLDocWriter::RecordRAII YAMLDocWriter::enterSubRecord(const char* name) {
  YAMLNode* curSub = m_subStack.back();
  if (curSub->m_type != YAML_MAPPING_NODE && curSub->m_type != YAML_SEQUENCE_NODE)
    return {};
  YAMLNode* newNode = new YAMLNode(YAML_MAPPING_NODE);
  if (curSub->m_type == YAML_MAPPING_NODE)
    curSub->assignMapChild(name ? name : std::string_view{}, std::unique_ptr<YAMLNode>(newNode));
  else if (curSub->m_type == YAML_SEQUENCE_NODE)
    curSub->m_seqChildren.emplace_back(newNode);
  m_subStack.push_back(newNode);
  return RecordRAII{this};
}

void YAMLDocWriter::_leaveSubRecord() {
  if (m_subStack.size() > 1) {
    YAMLNode* curSub = m_subStack.back();
    /* Automatically lower to scalar or sequence if there's only one unnamed node */
    if (curSub->m_mapChildren.size() == 1 && curSub->m_mapChildren[0].first.empty()) {
      auto& item = curSub->m_mapChildren[0];
      if (item.first.empty()) {
        if (item.second->m_type == YAML_SCALAR_NODE) {
          curSub->m_type = YAML_SCALAR_NODE;
          curSub->m_scalarString = std::move(item.second->m_scalarString);
          curSub->m_mapChildren.clear();
        } else if (item.second->m_type == YAML_SEQUENCE_NODE) {
          curSub->m_type = YAML_SEQUENCE_NODE;
          curSub->m_seqChildren = std::move(item.second->m_seqChildren);
          curSub->m_mapChildren.clear();
        }
      }
    }
    m_subStack.pop_back();
  }
}

YAMLDocWriter::VectorRAII YAMLDocWriter::enterSubVector(const char* name) {
  YAMLNode* curSub = m_subStack.back();
  if (curSub->m_type != YAML_MAPPING_NODE && curSub->m_type != YAML_SEQUENCE_NODE)
    return {};
  YAMLNode* newNode = new YAMLNode(YAML_SEQUENCE_NODE);
  if (curSub->m_type == YAML_MAPPING_NODE)
    curSub->assignMapChild(name ? name : std::string_view{}, std::unique_ptr<YAMLNode>(newNode));
  else if (curSub->m_type == YAML_SEQUENCE_NODE)
    curSub->m_seqChildren.emplace_back(newNode);
  m_subStack.push_back(newNode);
  return VectorRAII{this};
}

void YAMLDocWriter::_leaveSubVector() {
  if (m_subStack.size() > 1)
    m_subStack.pop_back();
}

template <typename INTYPE>
void YAMLDocWriter::writeVal(const char* name, const INTYPE& val) {
  YAMLNode* curSub = m_subStack.back();
  if (curSub->m_type == YAML_MAPPING_NODE)
    curSub->assignMapChild(name ? name : std::string_view{}, std::move(ValToNode(val)));
  else if (curSub->m_type == YAML_SEQUENCE_NODE)
    curSub->m_seqChildren.emplace_back(std::move(ValToNode(val)));
}

template void YAMLDocWriter::writeVal<atInt8>(const char* name, const atInt8& val);
template void YAMLDocWriter::writeVal<atUint8>(const char* name, const atUint8& val);
template void YAMLDocWriter::writeVal<atInt16>(const char* name, const atInt16& val);
template void YAMLDocWriter::writeVal<atUint16>(const char* name, const atUint16& val);
template void YAMLDocWriter::writeVal<atInt32>(const char* name, const atInt32& val);
template void YAMLDocWriter::writeVal<atUint32>(const char* name, const atUint32& val);
template void YAMLDocWriter::writeVal<float>(const char* name, const float& val);
template void YAMLDocWriter::writeVal<double>(const char* name, const double& val);
template void YAMLDocWriter::writeVal<atVec3f>(const char* name, const atVec3f& val);
template void YAMLDocWriter::writeVal<atVec4f>(const char* name, const atVec4f& val);
template void YAMLDocWriter::writeVal<bool>(const char* name, const bool& val);

template <typename INTYPE>
void YAMLDocWriter::writeVal(const char* name, const INTYPE& val, size_t byteCount) {
  YAMLNode* curSub = m_subStack.back();
  if (curSub->m_type == YAML_MAPPING_NODE)
    curSub->assignMapChild(name ? name : std::string_view{}, std::move(ValToNode(val, byteCount)));
  else if (curSub->m_type == YAML_SEQUENCE_NODE)
    curSub->m_seqChildren.emplace_back(std::move(ValToNode(val, byteCount)));
}

void YAMLDocWriter::writeBool(const char* name, const bool& val) { writeVal<bool>(name, val); }

void YAMLDocWriter::writeByte(const char* name, const atInt8& val) { writeVal<atInt8>(name, val); }

void YAMLDocWriter::writeUByte(const char* name, const atUint8& val) { writeVal<atUint8>(name, val); }

void YAMLDocWriter::writeInt16(const char* name, const atInt16& val) { writeVal<atInt16>(name, val); }

void YAMLDocWriter::writeUint16(const char* name, const atUint16& val) { writeVal<atUint16>(name, val); }

void YAMLDocWriter::writeInt32(const char* name, const atInt32& val) { writeVal<atInt32>(name, val); }

void YAMLDocWriter::writeUint32(const char* name, const atUint32& val) { writeVal<atUint32>(name, val); }

void YAMLDocWriter::writeInt64(const char* name, const atInt64& val) { writeVal<atInt64>(name, val); }

void YAMLDocWriter::writeUint64(const char* name, const atUint64& val) { writeVal<atUint64>(name, val); }

void YAMLDocWriter::writeFloat(const char* name, const float& val) { writeVal<float>(name, val); }

void YAMLDocWriter::writeDouble(const char* name, const double& val) { writeVal<double>(name, val); }

void YAMLDocWriter::writeVec2f(const char* name, const atVec2f& val) { writeVal<atVec2f>(name, val); }

void YAMLDocWriter::writeVec3f(const char* name, const atVec3f& val) { writeVal<atVec3f>(name, val); }

void YAMLDocWriter::writeVec4f(const char* name, const atVec4f& val) { writeVal<atVec4f>(name, val); }

void YAMLDocWriter::writeVec2d(const char* name, const atVec2d& val) { writeVal<atVec2d>(name, val); }

void YAMLDocWriter::writeVec3d(const char* name, const atVec3d& val) { writeVal<atVec3d>(name, val); }

void YAMLDocWriter::writeVec4d(const char* name, const atVec4d& val) { writeVal<atVec4d>(name, val); }

void YAMLDocWriter::writeUBytes(const char* name, const std::unique_ptr<atUint8[]>& val, size_t byteCount) {
  writeVal<const std::unique_ptr<atUint8[]>&>(name, val, byteCount);
}

void YAMLDocWriter::writeString(const char* name, std::string_view val) { writeVal<std::string_view>(name, val); }

void YAMLDocWriter::writeWString(const char* name, std::wstring_view val) { writeVal<std::wstring_view>(name, val); }

void YAMLDocWriter::writeU16String(const char* name, std::u16string_view val) {
  writeVal<std::u16string_view>(name, val);
}

void YAMLDocWriter::writeU32String(const char* name, std::u32string_view val) {
  writeVal<std::u32string_view>(name, val);
}

void YAMLDocWriter::setStyle(YAMLNodeStyle s) { m_subStack.back()->m_style = s; }

static void InsertNode(std::vector<YAMLNode*>& nodeStack, std::unique_ptr<YAMLNode>& mapKey,
                       std::unique_ptr<YAMLNode>& retVal, std::unique_ptr<YAMLNode>&& newNode) {
  if (nodeStack.empty()) {
    retVal = std::move(newNode);
    return;
  }
  YAMLNode* parent = nodeStack.back();
  if (parent->m_type == YAML_SEQUENCE_NODE) {
    parent->m_seqChildren.emplace_back(std::move(newNode));
  } else if (parent->m_type == YAML_MAPPING_NODE) {
    if (!mapKey)
      mapKey = std::move(newNode);
    else {
      parent->assignMapChild(mapKey->m_scalarString, std::move(newNode));
      mapKey.reset(nullptr);
    }
  }
}

std::unique_ptr<YAMLNode> YAMLDocReader::ParseEvents(athena::io::IStreamReader* reader) {
  yaml_event_t event;
  if (reader)
    yaml_parser_set_input(&m_parser, (yaml_read_handler_t*)YAMLAthenaReader, reader);
  if (!yaml_parser_parse(&m_parser, &event)) {
    HandleYAMLParserError(&m_parser);
    return std::unique_ptr<YAMLNode>();
  }

  std::vector<YAMLNode*> nodeStack;
  std::unique_ptr<YAMLNode> mapKey;
  std::unique_ptr<YAMLNode> retVal;
  int result;
  for (result = yaml_parser_parse(&m_parser, &event); event.type != YAML_STREAM_END_EVENT;
       result = yaml_parser_parse(&m_parser, &event)) {
    if (!result) {
      HandleYAMLParserError(&m_parser);
      return nullptr;
    }
    switch (event.type) {
    case YAML_SCALAR_EVENT: {
      if (nodeStack.empty()) {
        atWarning(fmt("YAML parser stack empty; skipping scalar node"));
        break;
      }
      auto newScalar = std::make_unique<YAMLNode>(YAML_SCALAR_NODE);
      newScalar->m_scalarString.assign((char*)event.data.scalar.value, event.data.scalar.length);
      if (nodeStack.empty())
        retVal = std::move(newScalar);
      else
        InsertNode(nodeStack, mapKey, retVal, std::move(newScalar));
      break;
    }
    case YAML_SEQUENCE_START_EVENT: {
      YAMLNode* newSeq = new YAMLNode(YAML_SEQUENCE_NODE);
      InsertNode(nodeStack, mapKey, retVal, std::unique_ptr<YAMLNode>(newSeq));
      nodeStack.emplace_back(newSeq);
      break;
    }
    case YAML_SEQUENCE_END_EVENT: {
      nodeStack.pop_back();
      break;
    }
    case YAML_MAPPING_START_EVENT: {
      YAMLNode* newMap = new YAMLNode(YAML_MAPPING_NODE);
      InsertNode(nodeStack, mapKey, retVal, std::unique_ptr<YAMLNode>(newMap));
      nodeStack.emplace_back(newMap);
      break;
    }
    case YAML_MAPPING_END_EVENT: {
      nodeStack.pop_back();
      break;
    }
    case YAML_DOCUMENT_END_EVENT: {
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

YAMLDocReader::YAMLDocReader() {
  if (!yaml_parser_initialize(&m_parser)) {
    HandleYAMLParserError(&m_parser);
    return;
  }
}
YAMLDocReader::~YAMLDocReader() { yaml_parser_delete(&m_parser); }

void YAMLDocReader::reset() {
  yaml_parser_delete(&m_parser);
  if (!yaml_parser_initialize(&m_parser))
    HandleYAMLParserError(&m_parser);
}

bool YAMLDocReader::parse(athena::io::IStreamReader* reader) {
  std::unique_ptr<YAMLNode> newRoot = ParseEvents(reader);
  if (!newRoot)
    return false;
  m_rootNode = std::move(newRoot);
  m_subStack.clear();
  m_subStack.push_back(m_rootNode.get());
  m_seqTrackerStack.clear();
  return true;
}

bool YAMLDocReader::ClassTypeOperation(std::function<bool(const char* dnaType)> func) {
  yaml_event_t event;
  if (!yaml_parser_parse(&m_parser, &event)) {
    HandleYAMLParserError(&m_parser);
    return false;
  }

  int result;
  int mappingLevel = 0;
  bool inDNA = false;
  for (result = yaml_parser_parse(&m_parser, &event); event.type != YAML_STREAM_END_EVENT;
       result = yaml_parser_parse(&m_parser, &event)) {
    if (!result) {
      HandleYAMLParserError(&m_parser);
      return false;
    }
    switch (event.type) {
    case YAML_SCALAR_EVENT: {
      if (mappingLevel == 1) {
        if (inDNA) {
          bool result = func(reinterpret_cast<const char*>(event.data.scalar.value));
          yaml_event_delete(&event);
          return result;
        }
        if (!strcmp("DNAType", reinterpret_cast<const char*>(event.data.scalar.value)))
          inDNA = true;
      }
      break;
    }
    case YAML_MAPPING_START_EVENT: {
      ++mappingLevel;
      inDNA = false;
      break;
    }
    case YAML_MAPPING_END_EVENT: {
      --mappingLevel;
      inDNA = false;
      break;
    }
    case YAML_DOCUMENT_END_EVENT: {
      yaml_event_delete(&event);
      return false;
    }
    default:
      break;
    }
    yaml_event_delete(&event);
  }
  return false;
}

bool YAMLDocReader::ValidateClassType(const char* expectedType) {
  if (!expectedType)
    return false;

  return ClassTypeOperation([&](const char* dnaType) -> bool { return (strcmp(expectedType, dnaType) == 0); });
}

YAMLDocReader::RecordRAII YAMLDocReader::enterSubRecord(const char* name) {
  YAMLNode* curSub = m_subStack.back();
  if (curSub->m_type == YAML_SEQUENCE_NODE) {
    int& seqIdx = m_seqTrackerStack.back();
    m_subStack.push_back(curSub->m_seqChildren[seqIdx++].get());
    if (m_subStack.back()->m_type == YAML_SEQUENCE_NODE)
      m_seqTrackerStack.push_back(0);
    return RecordRAII{this};
  } else if (!name) {
    atError(fmt("Expected YAML sequence"));
  }
  for (const auto& item : curSub->m_mapChildren) {
    if (!item.first.compare(name)) {
      m_subStack.push_back(item.second.get());
      if (m_subStack.back()->m_type == YAML_SEQUENCE_NODE)
        m_seqTrackerStack.push_back(0);
      return RecordRAII{this};
    }
  }
  return {};
}

void YAMLDocReader::_leaveSubRecord() {
  if (m_subStack.size() > 1) {
    if (m_subStack.back()->m_type == YAML_SEQUENCE_NODE)
      m_seqTrackerStack.pop_back();
    m_subStack.pop_back();
  }
}

YAMLDocReader::VectorRAII YAMLDocReader::enterSubVector(const char* name, size_t& countOut) {
  YAMLNode* curSub = m_subStack.back();
  if (!name && curSub->m_type == YAML_SEQUENCE_NODE) {
    m_subStack.push_back(curSub);
    m_seqTrackerStack.push_back(0);
    countOut = curSub->m_seqChildren.size();
    return VectorRAII{this};
  } else {
    for (const auto& item : curSub->m_mapChildren) {
      if (!item.first.compare(name)) {
        YAMLNode* nextSub = item.second.get();
        if (nextSub->m_type == YAML_SEQUENCE_NODE) {
          countOut = nextSub->m_seqChildren.size();
        } else {
          atError(fmt("'{}' is not a vector field"), name);
          countOut = 0;
        }
        m_subStack.push_back(nextSub);
        m_seqTrackerStack.push_back(0);
        return VectorRAII{this};
      }
    }
  }
  countOut = 0;
  return {};
}

void YAMLDocReader::_leaveSubVector() {
  if (m_subStack.size() > 1) {
    m_subStack.pop_back();
    m_seqTrackerStack.pop_back();
  }
}

template <typename RETURNTYPE>
RETURNTYPE YAMLDocReader::readVal(const char* name) {
  if (m_subStack.size()) {
    const YAMLNode* mnode = m_subStack.back();
    if (mnode->m_type == YAML_SCALAR_NODE) {
      return NodeToVal<RETURNTYPE>(mnode);
    } else if (mnode->m_type == YAML_SEQUENCE_NODE) {
      int& seqIdx = m_seqTrackerStack.back();
      return NodeToVal<RETURNTYPE>(mnode->m_seqChildren[seqIdx++].get());
    } else if (mnode->m_type == YAML_MAPPING_NODE && name) {
      for (const auto& item : mnode->m_mapChildren) {
        if (!item.first.compare(name)) {
          return NodeToVal<RETURNTYPE>(item.second.get());
        }
      }
    }
  }
  if (name)
    atWarning(fmt("Unable to find field '{}'; returning 0"), name);
  return RETURNTYPE();
}

template atInt8 YAMLDocReader::readVal<atInt8>(const char* name);
template atUint8 YAMLDocReader::readVal<atUint8>(const char* name);
template atInt16 YAMLDocReader::readVal<atInt16>(const char* name);
template atUint16 YAMLDocReader::readVal<atUint16>(const char* name);
template atInt32 YAMLDocReader::readVal<atInt32>(const char* name);
template atUint32 YAMLDocReader::readVal<atUint32>(const char* name);
template float YAMLDocReader::readVal<float>(const char* name);
template double YAMLDocReader::readVal<double>(const char* name);
template atVec3f YAMLDocReader::readVal<atVec3f>(const char* name);
template atVec4f YAMLDocReader::readVal<atVec4f>(const char* name);
template bool YAMLDocReader::readVal<bool>(const char* name);

bool YAMLDocReader::readBool(const char* name) { return readVal<bool>(name); }

atInt8 YAMLDocReader::readByte(const char* name) { return readVal<atInt8>(name); }

atUint8 YAMLDocReader::readUByte(const char* name) { return readVal<atUint8>(name); }

atInt16 YAMLDocReader::readInt16(const char* name) { return readVal<atInt16>(name); }

atUint16 YAMLDocReader::readUint16(const char* name) { return readVal<atUint16>(name); }

atInt32 YAMLDocReader::readInt32(const char* name) { return readVal<atInt32>(name); }

atUint32 YAMLDocReader::readUint32(const char* name) { return readVal<atUint32>(name); }

atInt64 YAMLDocReader::readInt64(const char* name) { return readVal<atInt64>(name); }

atUint64 YAMLDocReader::readUint64(const char* name) { return readVal<atUint64>(name); }

float YAMLDocReader::readFloat(const char* name) { return readVal<float>(name); }

double YAMLDocReader::readDouble(const char* name) { return readVal<double>(name); }

atVec2f YAMLDocReader::readVec2f(const char* name) { return readVal<atVec2f>(name); }

atVec3f YAMLDocReader::readVec3f(const char* name) { return readVal<atVec3f>(name); }
atVec4f YAMLDocReader::readVec4f(const char* name) { return readVal<atVec4f>(name); }

atVec2d YAMLDocReader::readVec2d(const char* name) { return readVal<atVec2d>(name); }

atVec3d YAMLDocReader::readVec3d(const char* name) { return readVal<atVec3d>(name); }

atVec4d YAMLDocReader::readVec4d(const char* name) { return readVal<atVec4d>(name); }

std::unique_ptr<atUint8[]> YAMLDocReader::readUBytes(const char* name) {
  return readVal<std::unique_ptr<atUint8[]>>(name);
}

std::string YAMLDocReader::readString(const char* name) { return readVal<std::string>(name); }

std::wstring YAMLDocReader::readWString(const char* name) { return readVal<std::wstring>(name); }

static bool EmitKeyScalar(yaml_emitter_t* doc, const char* val) {
  yaml_event_t event;
  if (!yaml_scalar_event_initialize(&event, nullptr, nullptr, (yaml_char_t*)val, strlen(val), true, true,
                                    YAML_PLAIN_SCALAR_STYLE))
    return false;
  return yaml_emitter_emit(doc, &event) != 0;
}

static yaml_scalar_style_t ScalarStyle(const YAMLNode& node) {
  for (const auto& ch : node.m_scalarString)
    if (ch == '\n')
      return YAML_LITERAL_SCALAR_STYLE;
  return YAML_ANY_SCALAR_STYLE;
}

static yaml_sequence_style_t SequenceStyle(const YAMLNode& node) {
  if (node.m_style == YAMLNodeStyle::Flow)
    return YAML_FLOW_SEQUENCE_STYLE;
  else if (node.m_style == YAMLNodeStyle::Block)
    return YAML_BLOCK_SEQUENCE_STYLE;

  size_t count = 0;
  for (const auto& item : node.m_seqChildren) {
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

static yaml_mapping_style_t MappingStyle(const YAMLNode& node) {
  if (node.m_style == YAMLNodeStyle::Flow)
    return YAML_FLOW_MAPPING_STYLE;
  else if (node.m_style == YAMLNodeStyle::Block)
    return YAML_BLOCK_MAPPING_STYLE;

  size_t count = 0;
  for (const auto& item : node.m_mapChildren) {
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

bool YAMLDocWriter::RecursiveFinish(yaml_emitter_t* doc, const YAMLNode& node) {
  const auto handleError = [doc] {
    HandleYAMLEmitterError(doc);
    return false;
  };

  yaml_event_t event;
  if (node.m_type == YAML_SCALAR_NODE) {
    if (!yaml_scalar_event_initialize(&event, nullptr, nullptr, (yaml_char_t*)node.m_scalarString.c_str(),
                                      node.m_scalarString.length(), true, true, ScalarStyle(node)) ||
        !yaml_emitter_emit(doc, &event)) {
      return handleError();
    }
  } else if (node.m_type == YAML_SEQUENCE_NODE) {
    if (!yaml_sequence_start_event_initialize(&event, nullptr, nullptr, 1, SequenceStyle(node)) ||
        !yaml_emitter_emit(doc, &event)) {
      return handleError();
    }
    for (const auto& item : node.m_seqChildren) {
      if (!RecursiveFinish(doc, *item)) {
        return handleError();
      }
    }
    if (!yaml_sequence_end_event_initialize(&event) || !yaml_emitter_emit(doc, &event)) {
      return handleError();
    }
  } else if (node.m_type == YAML_MAPPING_NODE) {
    if (!yaml_mapping_start_event_initialize(&event, nullptr, nullptr, true, MappingStyle(node)) ||
        !yaml_emitter_emit(doc, &event)) {
      return handleError();
    }
    for (const auto& item : node.m_mapChildren) {
      if (!EmitKeyScalar(doc, item.first.c_str())) {
        return handleError();
      }
      if (!RecursiveFinish(doc, *item.second)) {
        return handleError();
      }
    }
    event.type = YAML_MAPPING_END_EVENT;
    if (!yaml_mapping_end_event_initialize(&event) || !yaml_emitter_emit(doc, &event)) {
      return handleError();
    }
  }
  return true;
}

constexpr std::string_view base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static bool is_base64(unsigned char c) { return isalnum(c) || c == '+' || c == '/'; }

std::string base64_encode(const atUint8* bytes_to_encode, size_t in_len) {
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

      for (i = 0; (i < 4); i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while ((i++ < 3))
      ret += '=';
  }

  return ret;
}
std::unique_ptr<atUint8[]> base64_decode(std::string_view encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::unique_ptr<atUint8[]> ret(new atUint8[in_len * 3 / 4]);
  atUint8* retBuf = ret.get();

  while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_];
    in_++;
    if (i == 4) {
      for (i = 0; i < 4; i++)
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
    for (j = i; j < 4; j++)
      char_array_4[j] = 0;

    for (j = 0; j < 4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++)
      *retBuf++ = char_array_3[j];
  }

  return ret;
}

} // namespace athena::io
