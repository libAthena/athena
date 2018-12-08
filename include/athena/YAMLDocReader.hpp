#pragma once

#include "YAMLCommon.hpp"

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

  inline yaml_parser_t* getParser() { return &m_parser; }

  bool parse(athena::io::IStreamReader* reader);

  bool ClassTypeOperation(std::function<bool(const char* dnaType)> func);
  bool ValidateClassType(const char* expectedType);

  inline const YAMLNode* getRootNode() const { return m_rootNode.get(); }
  inline const YAMLNode* getCurNode() const { return m_subStack.empty() ? nullptr : m_subStack.back(); }
  std::unique_ptr<YAMLNode> releaseRootNode() { return std::move(m_rootNode); }

  class RecordRAII {
    friend class YAMLDocReader;
    YAMLDocReader* m_r = nullptr;
    RecordRAII(YAMLDocReader* r) : m_r(r) {}
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

  RecordRAII enterSubRecord(const char* name);

  template <class T>
  void enumerate(const char* name, T& record, typename std::enable_if_t<__IsDNARecord_v<T>>* = 0) {
    if (auto rec = enterSubRecord(name))
      record.read(*this);
  }

  class VectorRAII {
    friend class YAMLDocReader;
    YAMLDocReader* m_r = nullptr;
    VectorRAII(YAMLDocReader* r) : m_r(r) {}
    VectorRAII() = default;

  public:
    operator bool() const { return m_r != nullptr; }
    ~VectorRAII() {
      if (m_r)
        m_r->_leaveSubVector();
    }
  };
  friend class VectorRAII;

  VectorRAII enterSubVector(const char* name, size_t& countOut);

  template <class T>
  size_t
  enumerate(const char* name, std::vector<T>& vector,
            typename std::enable_if_t<!std::is_arithmetic<T>::value && !std::is_same<T, atVec2f>::value &&
                                      !std::is_same<T, atVec3f>::value && !std::is_same<T, atVec4f>::value>* = 0) {
    size_t countOut;
    if (auto v = enterSubVector(name, countOut)) {
      vector.clear();
      vector.reserve(countOut);
      for (size_t i = 0; i < countOut; ++i) {
        vector.emplace_back();
        if (auto rec = enterSubRecord(nullptr))
          vector.back().read(*this);
      }
    }
    return countOut;
  }

  template <class T>
  size_t enumerate(const char* name, std::vector<T>& vector,
                   typename std::enable_if_t<std::is_arithmetic<T>::value || std::is_same<T, atVec2f>::value ||
                                             std::is_same<T, atVec3f>::value || std::is_same<T, atVec4f>::value>* = 0) {
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
  size_t enumerate(const char* name, std::vector<T>& vector, std::function<void(YAMLDocReader&, T&)> readf) {
    size_t countOut;
    if (auto v = enterSubVector(name, countOut)) {
      vector.clear();
      vector.reserve(countOut);
      for (size_t i = 0; i < countOut; ++i) {
        vector.emplace_back();
        if (auto rec = enterSubRecord(nullptr))
          readf(*this, vector.back());
      }
    }
    return countOut;
  }

  bool hasVal(const char* name) const {
    if (m_subStack.size()) {
      const YAMLNode* mnode = m_subStack.back();
      if (mnode->m_type == YAML_MAPPING_NODE && name)
        for (const auto& item : mnode->m_mapChildren)
          if (!item.first.compare(name))
            return true;
    }
    return false;
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

} // namespace athena::io
