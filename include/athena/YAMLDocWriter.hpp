#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "athena/Types.hpp"
#include "athena/YAMLCommon.hpp"

namespace athena::io {

class YAMLDocWriter {
  std::unique_ptr<YAMLNode> m_rootNode;
  std::vector<YAMLNode*> m_subStack;
  yaml_emitter_t m_emitter;
  static bool RecursiveFinish(yaml_emitter_t* doc, const YAMLNode& node);
  void _leaveSubRecord();
  void _leaveSubVector();

public:
  explicit YAMLDocWriter(std::string_view classType, athena::io::IStreamReader* reader = nullptr);
  explicit YAMLDocWriter(athena::io::IStreamReader* reader = nullptr) : YAMLDocWriter({}, reader) {}
  ~YAMLDocWriter();

  yaml_emitter_t* getEmitter() { return &m_emitter; }

  bool finish(athena::io::IStreamWriter* fout);

  YAMLNode* getCurNode() const { return m_subStack.empty() ? nullptr : m_subStack.back(); }

  class RecordRAII {
    friend class YAMLDocWriter;
    YAMLDocWriter* m_w = nullptr;
    explicit RecordRAII(YAMLDocWriter* w) : m_w(w) {}
    RecordRAII() = default;

  public:
    operator bool() const { return m_w != nullptr; }
    ~RecordRAII() {
      if (m_w)
        m_w->_leaveSubRecord();
    }
  };
  friend class RecordRAII;

  RecordRAII enterSubRecord(std::string_view name = {});

  class VectorRAII {
    friend class YAMLDocWriter;
    YAMLDocWriter* m_w = nullptr;
    explicit VectorRAII(YAMLDocWriter* w) : m_w(w) {}
    VectorRAII() = default;

  public:
    operator bool() const { return m_w != nullptr; }
    ~VectorRAII() {
      if (m_w)
        m_w->_leaveSubVector();
    }
  };
  friend class VectorRAII;

  VectorRAII enterSubVector(std::string_view name = {});

  template <class T>
  void
  enumerate(std::string_view name, const std::vector<T>& vector,
            std::enable_if_t<!std::is_arithmetic_v<T> && !std::is_same_v<T, atVec2f> && !std::is_same_v<T, atVec3f> &&
                             !std::is_same_v<T, atVec4f> && !std::is_same_v<T, atVec2d> &&
                             !std::is_same_v<T, atVec3d> && !std::is_same_v<T, atVec4d>>* = nullptr) {
    if (auto v = enterSubVector(name))
      for (const T& item : vector)
        if (auto rec = enterSubRecord())
          item.write(*this);
  }

  template <class T>
  void enumerate(std::string_view name, const std::vector<T>& vector,
                 std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<T, atVec2f> || std::is_same_v<T, atVec3f> ||
                                  std::is_same_v<T, atVec4f> || std::is_same_v<T, atVec2d> ||
                                  std::is_same_v<T, atVec3d> || std::is_same_v<T, atVec4d>>* = nullptr) {
    if (auto v = enterSubVector(name))
      for (T item : vector)
        writeVal<T>(item);
  }

  template <class T>
  void enumerate(std::string_view name, const std::vector<T>& vector, std::function<void(YAMLDocWriter&, const T&)> writef) {
    if (auto v = enterSubVector(name))
      for (const T& item : vector)
        if (auto rec = enterSubRecord())
          writef(*this, item);
  }

  template <typename INTYPE>
  void writeVal(std::string_view name, const INTYPE& val);
  template <typename INTYPE>
  void writeVal(const INTYPE& val) { writeVal<INTYPE>(std::string_view{}, val); }
  template <typename INTYPE>
  void writeVal(std::string_view name, const INTYPE& val, size_t byteCount);
  template <typename INTYPE>
  void writeVal(const INTYPE& val, size_t byteCount) { writeVal<INTYPE>(std::string_view{}, val, byteCount); }
  void writeBool(std::string_view name, const bool& val);
  void writeBool(const bool& val) { writeBool({}, val); }
  void writeByte(std::string_view name, const atInt8& val);
  void writeByte(const atInt8& val) { writeByte({}, val); }
  void writeUByte(std::string_view name, const atUint8& val);
  void writeUByte(const atUint8& val) { writeUByte({}, val); }
  void writeInt16(std::string_view name, const atInt16& val);
  void writeInt16(const atInt16& val) { writeInt16({}, val); }
  void writeUint16(std::string_view name, const atUint16& val);
  void writeUint16(const atUint16& val) { writeUint16({}, val); }
  void writeInt32(std::string_view name, const atInt32& val);
  void writeInt32(const atInt32& val) { writeInt32({}, val); }
  void writeUint32(std::string_view name, const atUint32& val);
  void writeUint32(const atUint32& val) { writeUint32({}, val); }
  void writeInt64(std::string_view name, const atInt64& val);
  void writeInt64(const atInt64& val) { writeInt64({}, val); }
  void writeUint64(std::string_view name, const atUint64& val);
  void writeUint64(const atUint64& val) { writeUint64({}, val); }
  void writeFloat(std::string_view name, const float& val);
  void writeFloat(const float& val) { writeFloat({}, val); }
  void writeDouble(std::string_view name, const double& val);
  void writeDouble(const double& val) { writeDouble({}, val); }
  void writeVec2f(std::string_view name, const atVec2f& val);
  void writeVec2f(const atVec2f& val) { writeVec2f({}, val); }
  void writeVec3f(std::string_view name, const atVec3f& val);
  void writeVec3f(const atVec3f& val) { writeVec3f({}, val); }
  void writeVec4f(std::string_view name, const atVec4f& val);
  void writeVec4f(const atVec4f& val) { writeVec4f({}, val); }
  void writeVec2d(std::string_view name, const atVec2d& val);
  void writeVec2d(const atVec2d& val) { writeVec2d({}, val); }
  void writeVec3d(std::string_view name, const atVec3d& val);
  void writeVec3d(const atVec3d& val) { writeVec3d({}, val); }
  void writeVec4d(std::string_view name, const atVec4d& val);
  void writeVec4d(const atVec4d& val) { writeVec4d({}, val); }
  void writeUBytes(std::string_view name, const std::unique_ptr<atUint8[]>& val, size_t byteCount);
  void writeUBytes(const std::unique_ptr<atUint8[]>& val, size_t byteCount) { writeUBytes({}, val, byteCount); }
  void writeString(std::string_view name, std::string_view val);
  void writeString(std::string_view val) { writeString({}, val); }
  void writeWString(std::string_view name, std::wstring_view val);
  void writeWString(std::wstring_view val) { writeWString({}, val); }
  void writeU16String(std::string_view name, std::u16string_view val);
  void writeU16String(std::u16string_view val) { writeU16String({}, val); }
  void writeU32String(std::string_view name, std::u32string_view val);
  void writeU32String(std::u32string_view val) { writeU32String({}, val); }

  void setStyle(YAMLNodeStyle s);
};

} // namespace athena::io
