#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "athena/Types.hpp"
#include "athena/YAMLCommon.hpp"

namespace athena::io {

class YAMLDocReader {
  std::unique_ptr<YAMLNode> m_rootNode;
  std::vector<YAMLNode*> m_subStack;
  std::vector<int> m_seqTrackerStack;
  yaml_parser_t m_parser;
  std::unique_ptr<YAMLNode> ParseEvents(athena::io::IStreamReader* reader);
  void _leaveSubRecord();
  void _leaveSubVector();

public:
  YAMLDocReader();
  ~YAMLDocReader();

  void reset();

  yaml_parser_t* getParser() { return &m_parser; }

  bool parse(athena::io::IStreamReader* reader);

  bool ClassTypeOperation(std::function<bool(std::string_view dnaType)> func);
  bool ValidateClassType(std::string_view expectedType);

  const YAMLNode* getRootNode() const { return m_rootNode.get(); }
  const YAMLNode* getCurNode() const { return m_subStack.empty() ? nullptr : m_subStack.back(); }
  std::unique_ptr<YAMLNode> releaseRootNode() { return std::move(m_rootNode); }

  class RecordRAII {
    friend class YAMLDocReader;
    YAMLDocReader* m_r = nullptr;
    explicit RecordRAII(YAMLDocReader* r) : m_r(r) {}
    RecordRAII() = default;

  public:
    operator bool() const { return m_r != nullptr; }
    void leave() {
      if (m_r) {
        m_r->_leaveSubRecord();
        m_r = nullptr;
      }
    }
    ~RecordRAII() {
      if (m_r)
        m_r->_leaveSubRecord();
    }
  };
  friend class RecordRAII;

  RecordRAII enterSubRecord(std::string_view name = {});

  class VectorRAII {
    friend class YAMLDocReader;
    YAMLDocReader* m_r = nullptr;
    explicit VectorRAII(YAMLDocReader* r) : m_r(r) {}
    VectorRAII() = default;

  public:
    explicit operator bool() const { return m_r != nullptr; }
    ~VectorRAII() {
      if (m_r)
        m_r->_leaveSubVector();
    }
  };
  friend class VectorRAII;

  VectorRAII enterSubVector(std::string_view name, size_t& countOut);
  VectorRAII enterSubVector(size_t& countOut) { return enterSubVector({}, countOut); }

  template <class T>
  size_t enumerate(std::string_view name, std::vector<T>& vector,
                   std::enable_if_t<!std::is_arithmetic_v<T> && !std::is_same_v<T, atVec2f> &&
                                    !std::is_same_v<T, atVec3f> && !std::is_same_v<T, atVec4f>>* = nullptr) {
    size_t countOut;
    if (auto v = enterSubVector(name, countOut)) {
      vector.clear();
      vector.reserve(countOut);
      for (size_t i = 0; i < countOut; ++i) {
        vector.emplace_back();
        if (auto rec = enterSubRecord())
          vector.back().read(*this);
      }
    }
    return countOut;
  }

  template <class T>
  size_t enumerate(std::string_view name, std::vector<T>& vector,
                   std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<T, atVec2f> ||
                                    std::is_same_v<T, atVec3f> || std::is_same_v<T, atVec4f>>* = nullptr) {
    size_t countOut;
    if (auto v = enterSubVector(name, countOut)) {
      vector.clear();
      vector.reserve(countOut);
      for (size_t i = 0; i < countOut; ++i)
        vector.push_back(readVal<T>(name));
    }
    return countOut;
  }

  template <class T>
  size_t enumerate(std::string_view name, std::vector<T>& vector, std::function<void(YAMLDocReader&, T&)> readf) {
    size_t countOut;
    if (auto v = enterSubVector(name, countOut)) {
      vector.clear();
      vector.reserve(countOut);
      for (size_t i = 0; i < countOut; ++i) {
        vector.emplace_back();
        if (auto rec = enterSubRecord())
          readf(*this, vector.back());
      }
    }
    return countOut;
  }

  bool hasVal(std::string_view name) const {
    if (m_subStack.size()) {
      const YAMLNode* mnode = m_subStack.back();
      if (mnode->m_type == YAML_MAPPING_NODE && !name.empty())
        for (const auto& item : mnode->m_mapChildren)
          if (item.first == name)
            return true;
    }
    return false;
  }

  template <typename RETURNTYPE>
  RETURNTYPE readVal(std::string_view name = {});
  bool readBool(std::string_view name = {});
  atInt8 readByte(std::string_view name = {});
  atUint8 readUByte(std::string_view name = {});
  atInt16 readInt16(std::string_view name = {});
  atUint16 readUint16(std::string_view name = {});
  atInt32 readInt32(std::string_view name = {});
  atUint32 readUint32(std::string_view name = {});
  atInt64 readInt64(std::string_view name = {});
  atUint64 readUint64(std::string_view name = {});
  float readFloat(std::string_view name = {});
  double readDouble(std::string_view name = {});
  atVec2f readVec2f(std::string_view name = {});
  atVec3f readVec3f(std::string_view name = {});
  atVec4f readVec4f(std::string_view name = {});
  atVec2d readVec2d(std::string_view name = {});
  atVec3d readVec3d(std::string_view name = {});
  atVec4d readVec4d(std::string_view name = {});
  std::unique_ptr<atUint8[]> readUBytes(std::string_view name = {});
  std::string readString(std::string_view name = {});
  std::wstring readWString(std::string_view name = {});
};

} // namespace athena::io
