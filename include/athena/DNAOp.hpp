#pragma once

#include "IStreamReader.hpp"
#include "IStreamWriter.hpp"
#include "YAMLDocReader.hpp"
#include "YAMLDocWriter.hpp"
#include "ChecksumsLiterals.hpp"
#include <type_traits>

namespace athena::io {

struct PropId {
  const char* name = nullptr;
  uint32_t rcrc32 = 0xffffffff;
  uint64_t crc64 = 0x0;
  template <class T>
  constexpr T opget() const;
  constexpr PropId() = default;
  constexpr PropId(const char* name, uint32_t rcrc32, uint64_t crc64) : name(name), rcrc32(rcrc32), crc64(crc64) {}
  constexpr PropId(const char* name)
  : name(name)
  , rcrc32(athena::checksums::literals::rcrc32_rec(0xFFFFFFFF, name))
  , crc64(athena::checksums::literals::crc64_rec(0xFFFFFFFFFFFFFFFF, name)) {}
  constexpr PropId(const char* name, uint32_t rcrc32)
  : name(name), rcrc32(rcrc32), crc64(athena::checksums::literals::crc64_rec(0xFFFFFFFFFFFFFFFF, name)) {}
};

template <>
constexpr uint32_t PropId::opget<uint32_t>() const {
  return rcrc32;
}

template <>
constexpr uint64_t PropId::opget<uint64_t>() const {
  return crc64;
}

namespace literals {
constexpr PropId operator"" _propid(const char* s, size_t len) { return {s}; }
} // namespace literals

#define AT_PROP_CASE(...) case athena::io::PropId(__VA_ARGS__).opget<typename Op::PropT>()

#if defined(__atdna__)
#define AT_OVERRIDE_RCRC32(rcrc32) __attribute__((annotate("rcrc32=" #rcrc32)))
#else
#define AT_OVERRIDE_RCRC32(rcrc32)
#endif

#if defined(__atdna__)
#define AT_SPECIALIZE_PARMS(...) __attribute__((annotate("specparms=" #__VA_ARGS__)))
#else
#define AT_SPECIALIZE_PARMS(...)
#endif

enum class PropType { None, CRC32, CRC64 };

template <class T>
using __IsPODType = typename std::disjunction<
    std::is_arithmetic<std::remove_cv_t<T>>, std::is_convertible<std::remove_cv_t<T>&, atVec2f&>,
    std::is_convertible<std::remove_cv_t<T>&, atVec3f&>, std::is_convertible<std::remove_cv_t<T>&, atVec4f&>,
    std::is_convertible<std::remove_cv_t<T>&, atVec2d&>, std::is_convertible<std::remove_cv_t<T>&, atVec3d&>,
    std::is_convertible<std::remove_cv_t<T>&, atVec4d&>>;
template <class T>
constexpr bool __IsPODType_v = __IsPODType<T>::value;

template <class T>
using __CastPODType = typename std::conditional_t<
    std::is_convertible_v<std::remove_cv_t<T>&, atVec2f&>, atVec2f,
    std::conditional_t<
        std::is_convertible_v<std::remove_cv_t<T>&, atVec3f&>, atVec3f,
        std::conditional_t<
            std::is_convertible_v<std::remove_cv_t<T>&, atVec4f&>, atVec4f,
            std::conditional_t<
                std::is_convertible_v<std::remove_cv_t<T>&, atVec2d&>, atVec2d,
                std::conditional_t<std::is_convertible_v<std::remove_cv_t<T>&, atVec3d&>, atVec3d,
                                   std::conditional_t<std::is_convertible_v<std::remove_cv_t<T>&, atVec4d&>, atVec4d,
                                                      std::remove_cv_t<T>>>>>>>;

template <Endian DNAE>
uint16_t __Read16(IStreamReader& r) {
  return DNAE == Endian::Big ? r.readUint16Big() : r.readUint16Little();
}

template <Endian DNAE>
void __Write16(IStreamWriter& w, uint16_t v) {
  DNAE == Endian::Big ? w.writeUint16Big(v) : w.writeUint16Little(v);
}

template <Endian DNAE>
uint32_t __Read32(IStreamReader& r) {
  return DNAE == Endian::Big ? r.readUint32Big() : r.readUint32Little();
}

template <Endian DNAE>
void __Write32(IStreamWriter& w, uint32_t v) {
  DNAE == Endian::Big ? w.writeUint32Big(v) : w.writeUint32Little(v);
}

template <Endian DNAE>
uint64_t __Read64(IStreamReader& r) {
  return DNAE == Endian::Big ? r.readUint64Big() : r.readUint64Little();
}

template <Endian DNAE>
void __Write64(IStreamWriter& w, uint64_t v) {
  DNAE == Endian::Big ? w.writeUint64Big(v) : w.writeUint64Little(v);
}

template <PropType PropOp>
struct BinarySize {
  using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
  using StreamT = size_t;
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_enum_v<T>> Do(const PropId& id, T& var, StreamT& s) {
    if (PropOp != PropType::None) {
      /* Accessed via Enumerate, header */
      s += 6;
    }
    using PODType = std::underlying_type_t<T>;
    BinarySize<PropType::None>::Do<PODType, DNAE>(id, *reinterpret_cast<PODType*>(&var), s);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsPODType_v<T>> Do(const PropId& id, T& var, StreamT& s) {
    if (PropOp != PropType::None) {
      /* Accessed via Enumerate, header */
      s += 6;
    }
    using CastT = __CastPODType<T>;
    BinarySize<PropType::None>::Do<CastT, DNAE>(id, static_cast<CastT&>(const_cast<std::remove_cv_t<T>&>(var)), s);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsDNARecord_v<T> && PropOp != PropType::None> Do(const PropId& id, T& var,
                                                                                      StreamT& s) {
    /* Accessed via Enumerate, header */
    s += 6;
    var.template Enumerate<BinarySize<PropOp>>(s);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsDNARecord_v<T> && PropOp == PropType::None> Do(const PropId& id, T& var,
                                                                                      StreamT& s) {
    var.template Enumerate<BinarySize<PropType::None>>(s);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_array_v<T>> Do(const PropId& id, T& var, StreamT& s) {
    for (auto& v : var)
      BinarySize<PropOp>::Do<std::remove_reference_t<decltype(v)>, DNAE>(id, v, s);
  }
  template <class T, Endian DNAE>
  static void DoSize(const PropId& id, T& var, StreamT& s) {
    BinarySize<PropOp>::Do<T, DNAE>(id, var, s);
  }
  template <class T, class S, Endian DNAE>
  static typename std::enable_if_t<!std::is_same_v<T, bool>> Do(const PropId& id, std::vector<T>& vector,
                                                                const S& count, StreamT& s) {
    for (T& v : vector)
      BinarySize<PropOp>::Do<T, DNAE>(id, v, s);
  }
  template <class T, class S, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, bool>> Do(const PropId& id, std::vector<T>& vector, const S& count,
                                                               StreamT& s) {
    /* libc++ specializes vector<bool> as a bitstream */
    s += vector.size();
  }
  static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& s) {
    if (buf)
      s += count;
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::string>> Do(const PropId& id, T& str, StreamT& s) {
    s += str.size() + 1;
  }
  static void Do(const PropId& id, std::string& str, atInt32 count, StreamT& s) {
    if (count < 0)
      s += str.size() + 1;
    else
      s += count;
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::wstring>> Do(const PropId& id, T& str, StreamT& s) {
    s += str.size() * 2 + 2;
  }
  template <Endian DNAE>
  static void Do(const PropId& id, std::wstring& str, atInt32 count, StreamT& s) {
    if (count < 0)
      s += str.size() * 2 + 2;
    else
      s += count * 2;
  }
  static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& s) {
    switch (whence) {
    case SeekOrigin::Begin:
      s = amount;
      break;
    case SeekOrigin::Current:
      s += amount;
      break;
    default:
      break;
    }
  }
  static void DoAlign(atInt64 amount, StreamT& s) { s = (s + amount - 1) / amount * amount; }
};
#define __BINARY_SIZE_S(type, endian)                                                                                  \
  template <>                                                                                                          \
  template <>                                                                                                          \
  inline void BinarySize<PropType::None>::Do<type, endian>(const PropId& id, type& var, BinarySize::StreamT& s)
__BINARY_SIZE_S(bool, Endian::Big) { s += 1; }
__BINARY_SIZE_S(atInt8, Endian::Big) { s += 1; }
__BINARY_SIZE_S(atUint8, Endian::Big) { s += 1; }
__BINARY_SIZE_S(atInt16, Endian::Big) { s += 2; }
__BINARY_SIZE_S(atUint16, Endian::Big) { s += 2; }
__BINARY_SIZE_S(atInt32, Endian::Big) { s += 4; }
__BINARY_SIZE_S(atUint32, Endian::Big) { s += 4; }
__BINARY_SIZE_S(atInt64, Endian::Big) { s += 8; }
__BINARY_SIZE_S(atUint64, Endian::Big) { s += 8; }
__BINARY_SIZE_S(float, Endian::Big) { s += 4; }
__BINARY_SIZE_S(double, Endian::Big) { s += 8; }
__BINARY_SIZE_S(atVec2f, Endian::Big) { s += 8; }
__BINARY_SIZE_S(atVec2d, Endian::Big) { s += 16; }
__BINARY_SIZE_S(atVec3f, Endian::Big) { s += 12; }
__BINARY_SIZE_S(atVec3d, Endian::Big) { s += 24; }
__BINARY_SIZE_S(atVec4f, Endian::Big) { s += 16; }
__BINARY_SIZE_S(atVec4d, Endian::Big) { s += 32; }
__BINARY_SIZE_S(bool, Endian::Little) { s += 1; }
__BINARY_SIZE_S(atInt8, Endian::Little) { s += 1; }
__BINARY_SIZE_S(atUint8, Endian::Little) { s += 1; }
__BINARY_SIZE_S(atInt16, Endian::Little) { s += 2; }
__BINARY_SIZE_S(atUint16, Endian::Little) { s += 2; }
__BINARY_SIZE_S(atInt32, Endian::Little) { s += 4; }
__BINARY_SIZE_S(atUint32, Endian::Little) { s += 4; }
__BINARY_SIZE_S(atInt64, Endian::Little) { s += 8; }
__BINARY_SIZE_S(atUint64, Endian::Little) { s += 8; }
__BINARY_SIZE_S(float, Endian::Little) { s += 4; }
__BINARY_SIZE_S(double, Endian::Little) { s += 8; }
__BINARY_SIZE_S(atVec2f, Endian::Little) { s += 8; }
__BINARY_SIZE_S(atVec2d, Endian::Little) { s += 16; }
__BINARY_SIZE_S(atVec3f, Endian::Little) { s += 12; }
__BINARY_SIZE_S(atVec3d, Endian::Little) { s += 24; }
__BINARY_SIZE_S(atVec4f, Endian::Little) { s += 16; }
__BINARY_SIZE_S(atVec4d, Endian::Little) { s += 32; }

template <PropType PropOp>
struct PropCount {
  using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
  using StreamT = size_t;
  template <class T, Endian DNAE>
  static void Do(const PropId& id, T& var, StreamT& s) {
    /* Only reports one level of properties */
    s += 1;
  }
  template <class T, Endian DNAE>
  static void DoSize(const PropId& id, T& var, StreamT& s) {
    PropCount<PropOp>::Do<T, DNAE>(id, var, s);
  }
  template <class T, class S, Endian DNAE>
  static void Do(const PropId& id, std::vector<T>& vector, const S& count, StreamT& s) {
    /* Only reports one level of properties */
    s += 1;
  }
  static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& s) {
    /* Only reports one level of properties */
    s += 1;
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::string>> Do(const PropId& id, T& str, StreamT& s) {
    /* Only reports one level of properties */
    s += 1;
  }
  static void Do(const PropId& id, std::string& str, atInt32 count, StreamT& s) {
    /* Only reports one level of properties */
    s += 1;
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::wstring>> Do(const PropId& id, T& str, StreamT& s) {
    /* Only reports one level of properties */
    s += 1;
  }
  template <Endian DNAE>
  static void Do(const PropId& id, std::wstring& str, atInt32 count, StreamT& s) {
    /* Only reports one level of properties */
    s += 1;
  }
  static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& s) {}
  static void DoAlign(atInt64 amount, StreamT& s) {}
};

template <PropType PropOp>
struct Read {
  using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
  using StreamT = IStreamReader;
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_enum_v<T>> Do(const PropId& id, T& var, StreamT& r) {
    using PODType = std::underlying_type_t<T>;
    Read<PropType::None>::Do<PODType, DNAE>(id, *reinterpret_cast<PODType*>(&var), r);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsPODType_v<T>> Do(const PropId& id, T& var, StreamT& r) {
    using CastT = __CastPODType<T>;
    Read<PropType::None>::Do<CastT, DNAE>(id, static_cast<CastT&>(var), r);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsDNARecord<T>() && PropOp == PropType::None> Do(const PropId& id, T& var,
                                                                                      StreamT& r) {
    var.template Enumerate<Read<PropType::None>>(r);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsDNARecord<T>() && PropOp != PropType::None> Do(const PropId& id, T& var,
                                                                                      StreamT& r) {
    /* Accessed via Lookup, no header */
    atUint16 propCount = __Read16<T::DNAEndian>(r);
    for (atUint32 i = 0; i < propCount; ++i) {
      atUint64 hash;
      if (PropOp == PropType::CRC64)
        hash = __Read64<T::DNAEndian>(r);
      else
        hash = __Read32<T::DNAEndian>(r);
      atInt64 size = __Read16<T::DNAEndian>(r);
      atInt64 start = r.position();
      var.template Lookup<Read<PropOp>>(hash, r);
      atInt64 actualRead = r.position() - start;
      if (actualRead != size)
        r.seek(size - actualRead);
    }
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_array_v<T>> Do(const PropId& id, T& var, StreamT& s) {
    for (auto& v : var)
      Read<PropOp>::Do<std::remove_reference_t<decltype(v)>, DNAE>(id, v, s);
  }
  template <class T, Endian DNAE>
  static void DoSize(const PropId& id, T& var, StreamT& s) {
    Read<PropOp>::Do<T, DNAE>(id, var, s);
  }
  template <class T, class S, Endian DNAE>
  static typename std::enable_if_t<!std::is_same_v<T, bool>> Do(const PropId& id, std::vector<T>& vector,
                                                                const S& count, StreamT& r) {
    vector.clear();
    vector.reserve(count);
    for (size_t i = 0; i < static_cast<size_t>(count); ++i) {
      vector.emplace_back();
      Read<PropOp>::Do<T, DNAE>(id, vector.back(), r);
    }
  }
  template <class T, class S, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, bool>> Do(const PropId& id, std::vector<T>& vector, const S& count,
                                                               StreamT& r) {
    /* libc++ specializes vector<bool> as a bitstream */
    vector.clear();
    vector.reserve(count);
    for (size_t i = 0; i < count; ++i)
      vector.push_back(r.readBool());
  }
  static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& r) {
    buf.reset(new atUint8[count]);
    r.readUBytesToBuf(buf.get(), count);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::string>> Do(const PropId& id, T& str, StreamT& r) {
    str = r.readString();
  }
  static void Do(const PropId& id, std::string& str, atInt32 count, StreamT& r) { str = r.readString(count); }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::wstring>> Do(const PropId& id, T& str, StreamT& r) {
    Read<PropType::None>::Do<DNAE>(id, str, r);
  }
  template <Endian DNAE>
  static void Do(const PropId& id, std::wstring& str, atInt32 count, StreamT& r) {
    Read<PropType::None>::Do<DNAE>(id, str, count, r);
  }
  static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& r) { r.seek(amount, whence); }
  static void DoAlign(atInt64 amount, StreamT& r) {
    r.seek((r.position() + amount - 1) / amount * amount, athena::Begin);
  }
};
#define __READ_S(type, endian)                                                                                         \
  template <>                                                                                                          \
  template <>                                                                                                          \
  inline void Read<PropType::None>::Do<type, endian>(const PropId& id, type& var, Read::StreamT& r)
#define __READ_WSTR_S(endian)                                                                                          \
  template <>                                                                                                          \
  template <>                                                                                                          \
  inline void Read<PropType::None>::Do<std::wstring, endian>(const PropId& id, std::wstring& str, Read::StreamT& r)
#define __READ_WSTRC_S(endian)                                                                                         \
  template <>                                                                                                          \
  template <>                                                                                                          \
  inline void Read<PropType::None>::Do<endian>(const PropId& id, std::wstring& str, atInt32 count, Read::StreamT& r)
__READ_S(bool, Endian::Big) { var = r.readBool(); }
__READ_S(atInt8, Endian::Big) { var = r.readByte(); }
__READ_S(atUint8, Endian::Big) { var = r.readUByte(); }
__READ_S(atInt16, Endian::Big) { var = r.readInt16Big(); }
__READ_S(atUint16, Endian::Big) { var = r.readUint16Big(); }
__READ_S(atInt32, Endian::Big) { var = r.readInt32Big(); }
__READ_S(atUint32, Endian::Big) { var = r.readUint32Big(); }
__READ_S(atInt64, Endian::Big) { var = r.readInt64Big(); }
__READ_S(atUint64, Endian::Big) { var = r.readUint64Big(); }
__READ_S(float, Endian::Big) { var = r.readFloatBig(); }
__READ_S(double, Endian::Big) { var = r.readDoubleBig(); }
__READ_S(atVec2f, Endian::Big) { var = r.readVec2fBig(); }
__READ_S(atVec2d, Endian::Big) { var = r.readVec2dBig(); }
__READ_S(atVec3f, Endian::Big) { var = r.readVec3fBig(); }
__READ_S(atVec3d, Endian::Big) { var = r.readVec3dBig(); }
__READ_S(atVec4f, Endian::Big) { var = r.readVec4fBig(); }
__READ_S(atVec4d, Endian::Big) { var = r.readVec4dBig(); }
__READ_WSTR_S(Endian::Big) { str = r.readWStringBig(); }
__READ_WSTRC_S(Endian::Big) { str = r.readWStringBig(count); }
__READ_S(bool, Endian::Little) { var = r.readBool(); }
__READ_S(atInt8, Endian::Little) { var = r.readByte(); }
__READ_S(atUint8, Endian::Little) { var = r.readUByte(); }
__READ_S(atInt16, Endian::Little) { var = r.readInt16Little(); }
__READ_S(atUint16, Endian::Little) { var = r.readUint16Little(); }
__READ_S(atInt32, Endian::Little) { var = r.readInt32Little(); }
__READ_S(atUint32, Endian::Little) { var = r.readUint32Little(); }
__READ_S(atInt64, Endian::Little) { var = r.readInt64Little(); }
__READ_S(atUint64, Endian::Little) { var = r.readUint64Little(); }
__READ_S(float, Endian::Little) { var = r.readFloatLittle(); }
__READ_S(double, Endian::Little) { var = r.readDoubleLittle(); }
__READ_S(atVec2f, Endian::Little) { var = r.readVec2fLittle(); }
__READ_S(atVec2d, Endian::Little) { var = r.readVec2dLittle(); }
__READ_S(atVec3f, Endian::Little) { var = r.readVec3fLittle(); }
__READ_S(atVec3d, Endian::Little) { var = r.readVec3dLittle(); }
__READ_S(atVec4f, Endian::Little) { var = r.readVec4fLittle(); }
__READ_S(atVec4d, Endian::Little) { var = r.readVec4dLittle(); }
__READ_WSTR_S(Endian::Little) { str = r.readWStringLittle(); }
__READ_WSTRC_S(Endian::Little) { str = r.readWStringLittle(count); }

template <PropType PropOp>
struct Write {
  using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
  using StreamT = IStreamWriter;
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_enum_v<T>> Do(const PropId& id, T& var, StreamT& w) {
    if (PropOp != PropType::None) {
      /* Accessed via Enumerate, header */
      if (PropOp == PropType::CRC64)
        __Write64<DNAE>(w, id.crc64);
      else
        __Write32<DNAE>(w, id.rcrc32);
      size_t binarySize = 0;
      BinarySize<PropType::None>::Do<T, DNAE>(id, var, binarySize);
      DNAE == Endian::Big ? w.writeUint16Big(atUint16(binarySize)) : w.writeUint16Little(atUint16(binarySize));
    }
    using PODType = std::underlying_type_t<T>;
    Write<PropType::None>::Do<PODType, DNAE>(id, *reinterpret_cast<PODType*>(&var), w);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsPODType_v<T>> Do(const PropId& id, T& var, StreamT& w) {
    using CastT = __CastPODType<T>;
    if (PropOp != PropType::None) {
      /* Accessed via Enumerate, header */
      if (PropOp == PropType::CRC64)
        __Write64<DNAE>(w, id.crc64);
      else
        __Write32<DNAE>(w, id.rcrc32);
      size_t binarySize = 0;
      BinarySize<PropType::None>::Do<CastT, DNAE>(id, static_cast<CastT&>(const_cast<std::remove_cv_t<T>&>(var)),
                                                  binarySize);
      __Write16<DNAE>(w, atUint16(binarySize));
    }
    Write<PropType::None>::Do<CastT, DNAE>(id, static_cast<CastT&>(const_cast<std::remove_cv_t<T>&>(var)), w);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsDNARecord<T>() && PropOp != PropType::None> Do(const PropId& id, T& var,
                                                                                      StreamT& w) {
    /* Accessed via Enumerate, header */
    if (PropOp == PropType::CRC64)
      __Write64<T::DNAEndian>(w, id.crc64);
    else
      __Write32<T::DNAEndian>(w, id.rcrc32);
    size_t binarySize = 0;
    var.template Enumerate<BinarySize<PropOp>>(binarySize);
    __Write16<T::DNAEndian>(w, atUint16(binarySize));

    size_t propCount = 0;
    var.template Enumerate<PropCount<PropOp>>(propCount);
    __Write16<T::DNAEndian>(w, atUint16(propCount));
    var.template Enumerate<Write<PropOp>>(w);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsDNARecord<T>() && PropOp == PropType::None> Do(const PropId& id, T& var,
                                                                                      StreamT& w) {
    var.template Enumerate<Write<PropType::None>>(w);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_array_v<T>> Do(const PropId& id, T& var, StreamT& s) {
    for (auto& v : var)
      Write<PropOp>::Do<std::remove_reference_t<decltype(v)>, DNAE>(id, v, s);
  }
  template <class T, Endian DNAE>
  static void DoSize(const PropId& id, T& var, StreamT& s) {
    Write<PropOp>::Do<T, DNAE>(id, var, s);
  }
  template <class T, class S, Endian DNAE>
  static typename std::enable_if_t<!std::is_same_v<T, bool>> Do(const PropId& id, std::vector<T>& vector,
                                                                const S& count, StreamT& w) {
    for (T& v : vector)
      Write<PropOp>::Do<T, DNAE>(id, v, w);
  }
  template <class T, class S, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, bool>> Do(const PropId& id, std::vector<T>& vector, const S& count,
                                                               StreamT& w) {
    /* libc++ specializes vector<bool> as a bitstream */
    for (const T& v : vector)
      w.writeBool(v);
  }
  static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& w) {
    if (buf)
      w.writeUBytes(buf.get(), count);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::string>> Do(const PropId& id, std::string& str, StreamT& w) {
    w.writeString(str);
  }
  static void Do(const PropId& id, std::string& str, atInt32 count, StreamT& w) { w.writeString(str, count); }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::wstring>> Do(const PropId& id, std::wstring& str,
                                                                       StreamT& w) {
    Write<PropType::None>::Do<DNAE>(id, str, w);
  }
  template <Endian DNAE>
  static void Do(const PropId& id, std::wstring& str, atInt32 count, StreamT& w) {
    Write<PropType::None>::Do<DNAE>(id, str, count, w);
  }
  static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& w) { w.seek(amount, whence); }
  static void DoAlign(atInt64 amount, StreamT& w) { w.writeZeroTo((w.position() + amount - 1) / amount * amount); }
};
#define __WRITE_S(type, endian)                                                                                        \
  template <>                                                                                                          \
  template <>                                                                                                          \
  inline void Write<PropType::None>::Do<type, endian>(const PropId& id, type& var, Write::StreamT& w)
#define __WRITE_WSTR_S(endian)                                                                                         \
  template <>                                                                                                          \
  template <>                                                                                                          \
  inline void Write<PropType::None>::Do<std::wstring, endian>(const PropId& id, std::wstring& str, Write::StreamT& w)
#define __WRITE_WSTRC_S(endian)                                                                                        \
  template <>                                                                                                          \
  template <>                                                                                                          \
  inline void Write<PropType::None>::Do<endian>(const PropId& id, std::wstring& str, atInt32 count, Write::StreamT& w)
__WRITE_S(bool, Endian::Big) { w.writeBool(var); }
__WRITE_S(atInt8, Endian::Big) { w.writeByte(var); }
__WRITE_S(atUint8, Endian::Big) { w.writeUByte(var); }
__WRITE_S(atInt16, Endian::Big) { w.writeInt16Big(var); }
__WRITE_S(atUint16, Endian::Big) { w.writeUint16Big(var); }
__WRITE_S(atInt32, Endian::Big) { w.writeInt32Big(var); }
__WRITE_S(atUint32, Endian::Big) { w.writeUint32Big(var); }
__WRITE_S(atInt64, Endian::Big) { w.writeInt64Big(var); }
__WRITE_S(atUint64, Endian::Big) { w.writeUint64Big(var); }
__WRITE_S(float, Endian::Big) { w.writeFloatBig(var); }
__WRITE_S(double, Endian::Big) { w.writeDoubleBig(var); }
__WRITE_S(atVec2f, Endian::Big) { w.writeVec2fBig(var); }
__WRITE_S(atVec2d, Endian::Big) { w.writeVec2dBig(var); }
__WRITE_S(atVec3f, Endian::Big) { w.writeVec3fBig(var); }
__WRITE_S(atVec3d, Endian::Big) { w.writeVec3dBig(var); }
__WRITE_S(atVec4f, Endian::Big) { w.writeVec4fBig(var); }
__WRITE_S(atVec4d, Endian::Big) { w.writeVec4dBig(var); }
__WRITE_WSTR_S(Endian::Big) { w.writeWStringBig(str); }
__WRITE_WSTRC_S(Endian::Big) { w.writeWStringBig(str, count); }
__WRITE_S(bool, Endian::Little) { w.writeBool(var); }
__WRITE_S(atInt8, Endian::Little) { w.writeByte(var); }
__WRITE_S(atUint8, Endian::Little) { w.writeUByte(var); }
__WRITE_S(atInt16, Endian::Little) { w.writeInt16Little(var); }
__WRITE_S(atUint16, Endian::Little) { w.writeUint16Little(var); }
__WRITE_S(atInt32, Endian::Little) { w.writeInt32Little(var); }
__WRITE_S(atUint32, Endian::Little) { w.writeUint32Little(var); }
__WRITE_S(atInt64, Endian::Little) { w.writeInt64Little(var); }
__WRITE_S(atUint64, Endian::Little) { w.writeUint64Little(var); }
__WRITE_S(float, Endian::Little) { w.writeFloatLittle(var); }
__WRITE_S(double, Endian::Little) { w.writeDoubleLittle(var); }
__WRITE_S(atVec2f, Endian::Little) { w.writeVec2fLittle(var); }
__WRITE_S(atVec2d, Endian::Little) { w.writeVec2dLittle(var); }
__WRITE_S(atVec3f, Endian::Little) { w.writeVec3fLittle(var); }
__WRITE_S(atVec3d, Endian::Little) { w.writeVec3dLittle(var); }
__WRITE_S(atVec4f, Endian::Little) { w.writeVec4fLittle(var); }
__WRITE_S(atVec4d, Endian::Little) { w.writeVec4dLittle(var); }
__WRITE_WSTR_S(Endian::Little) { w.writeWStringLittle(str); }
__WRITE_WSTRC_S(Endian::Little) { w.writeWStringLittle(str, count); }

template <PropType PropOp>
struct ReadYaml {
  using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
  using StreamT = YAMLDocReader;
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_enum_v<T>> Do(const PropId& id, T& var, StreamT& r) {
    using PODType = std::underlying_type_t<T>;
    ReadYaml<PropType::None>::Do<PODType, DNAE>(id, *reinterpret_cast<PODType*>(&var), r);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsPODType_v<T>> Do(const PropId& id, T& var, StreamT& r) {
    using CastT = __CastPODType<T>;
    ReadYaml<PropType::None>::Do<CastT, DNAE>(id, static_cast<CastT&>(var), r);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsDNARecord_v<T>> Do(const PropId& id, T& var, StreamT& r) {
    if (auto rec = r.enterSubRecord(id.name))
      var.template Enumerate<ReadYaml<PropOp>>(r);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_array_v<T>> Do(const PropId& id, T& var, StreamT& r) {
    size_t _count;
    if (auto __v = r.enterSubVector(id.name, _count))
      for (size_t i = 0; i < _count && i < std::extent_v<T>; ++i)
        ReadYaml<PropOp>::Do<std::remove_reference_t<decltype(var[i])>, DNAE>({}, var[i], r);
  }
  template <class T, Endian DNAE>
  static void DoSize(const PropId& id, T& var, StreamT& s) {
    /* Squelch size field access */
  }
  template <class T, class S, Endian DNAE>
  static typename std::enable_if_t<!std::is_same_v<T, bool>> Do(const PropId& id, std::vector<T>& vector,
                                                                const S& count, StreamT& r) {
    size_t _count;
    vector.clear();
    if (auto __v = r.enterSubVector(id.name, _count)) {
      vector.reserve(_count);
      for (size_t i = 0; i < _count; ++i) {
        vector.emplace_back();
        ReadYaml<PropOp>::Do<T, DNAE>({}, vector.back(), r);
      }
    }
    /* Horrible reference abuse (but it works) */
    const_cast<S&>(count) = vector.size();
  }
  template <class T, class S, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, bool>> Do(const PropId& id, std::vector<T>& vector, const S& count,
                                                               StreamT& r) {
    /* libc++ specializes vector<bool> as a bitstream */
    size_t _count;
    vector.clear();
    if (auto __v = r.enterSubVector(id.name, _count)) {
      vector.reserve(_count);
      for (size_t i = 0; i < _count; ++i)
        vector.push_back(r.readBool(nullptr));
    }
    /* Horrible reference abuse (but it works) */
    const_cast<S&>(count) = vector.size();
  }
  static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& r) {
    buf = r.readUBytes(id.name);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::string>> Do(const PropId& id, T& str, StreamT& r) {
    str = r.readString(id.name);
  }
  static void Do(const PropId& id, std::string& str, atInt32 count, StreamT& r) { str = r.readString(id.name); }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::wstring>> Do(const PropId& id, T& str, StreamT& r) {
    str = r.readWString(id.name);
  }
  template <Endian DNAE>
  static void Do(const PropId& id, std::wstring& str, atInt32 count, StreamT& r) {
    str = r.readWString(id.name);
  }
  static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& r) {}
  static void DoAlign(atInt64 amount, StreamT& r) {}
};
#define __READ_YAML_S(type, endian)                                                                                    \
  template <>                                                                                                          \
  template <>                                                                                                          \
  inline void ReadYaml<PropType::None>::Do<type, endian>(const PropId& id, type& var, ReadYaml::StreamT& r)
__READ_YAML_S(bool, Endian::Big) { var = r.readBool(id.name); }
__READ_YAML_S(atInt8, Endian::Big) { var = r.readByte(id.name); }
__READ_YAML_S(atUint8, Endian::Big) { var = r.readUByte(id.name); }
__READ_YAML_S(atInt16, Endian::Big) { var = r.readInt16(id.name); }
__READ_YAML_S(atUint16, Endian::Big) { var = r.readUint16(id.name); }
__READ_YAML_S(atInt32, Endian::Big) { var = r.readInt32(id.name); }
__READ_YAML_S(atUint32, Endian::Big) { var = r.readUint32(id.name); }
__READ_YAML_S(atInt64, Endian::Big) { var = r.readInt64(id.name); }
__READ_YAML_S(atUint64, Endian::Big) { var = r.readUint64(id.name); }
__READ_YAML_S(float, Endian::Big) { var = r.readFloat(id.name); }
__READ_YAML_S(double, Endian::Big) { var = r.readDouble(id.name); }
__READ_YAML_S(atVec2f, Endian::Big) { var = r.readVec2f(id.name); }
__READ_YAML_S(atVec2d, Endian::Big) { var = r.readVec2d(id.name); }
__READ_YAML_S(atVec3f, Endian::Big) { var = r.readVec3f(id.name); }
__READ_YAML_S(atVec3d, Endian::Big) { var = r.readVec3d(id.name); }
__READ_YAML_S(atVec4f, Endian::Big) { var = r.readVec4f(id.name); }
__READ_YAML_S(atVec4d, Endian::Big) { var = r.readVec4d(id.name); }
__READ_YAML_S(bool, Endian::Little) { var = r.readBool(id.name); }
__READ_YAML_S(atInt8, Endian::Little) { var = r.readByte(id.name); }
__READ_YAML_S(atUint8, Endian::Little) { var = r.readUByte(id.name); }
__READ_YAML_S(atInt16, Endian::Little) { var = r.readInt16(id.name); }
__READ_YAML_S(atUint16, Endian::Little) { var = r.readUint16(id.name); }
__READ_YAML_S(atInt32, Endian::Little) { var = r.readInt32(id.name); }
__READ_YAML_S(atUint32, Endian::Little) { var = r.readUint32(id.name); }
__READ_YAML_S(atInt64, Endian::Little) { var = r.readInt64(id.name); }
__READ_YAML_S(atUint64, Endian::Little) { var = r.readUint64(id.name); }
__READ_YAML_S(float, Endian::Little) { var = r.readFloat(id.name); }
__READ_YAML_S(double, Endian::Little) { var = r.readDouble(id.name); }
__READ_YAML_S(atVec2f, Endian::Little) { var = r.readVec2f(id.name); }
__READ_YAML_S(atVec2d, Endian::Little) { var = r.readVec2d(id.name); }
__READ_YAML_S(atVec3f, Endian::Little) { var = r.readVec3f(id.name); }
__READ_YAML_S(atVec3d, Endian::Little) { var = r.readVec3d(id.name); }
__READ_YAML_S(atVec4f, Endian::Little) { var = r.readVec4f(id.name); }
__READ_YAML_S(atVec4d, Endian::Little) { var = r.readVec4d(id.name); }

template <PropType PropOp>
struct WriteYaml {
  using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
  using StreamT = YAMLDocWriter;
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_enum_v<T>> Do(const PropId& id, T& var, StreamT& w) {
    using PODType = std::underlying_type_t<T>;
    WriteYaml<PropType::None>::Do<PODType, DNAE>(id, *reinterpret_cast<PODType*>(&var), w);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsPODType_v<T>> Do(const PropId& id, T& var, StreamT& w) {
    using CastT = __CastPODType<T>;
    WriteYaml<PropType::None>::Do<CastT, DNAE>(id, static_cast<CastT&>(const_cast<std::remove_cv_t<T>&>(var)), w);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<__IsDNARecord_v<T>> Do(const PropId& id, T& var, StreamT& w) {
    if (auto rec = w.enterSubRecord(id.name))
      var.template Enumerate<WriteYaml<PropOp>>(w);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_array_v<T>> Do(const PropId& id, T& var, StreamT& w) {
    if (auto __v = w.enterSubVector(id.name))
      for (auto& v : var)
        WriteYaml<PropOp>::Do<std::remove_reference_t<decltype(v)>, DNAE>({}, v, w);
  }
  template <class T, Endian DNAE>
  static void DoSize(const PropId& id, T& var, StreamT& s) {
    /* Squelch size field access */
  }
  template <class T, class S, Endian DNAE>
  static typename std::enable_if_t<!std::is_same_v<T, bool>> Do(const PropId& id, std::vector<T>& vector,
                                                                const S& count, StreamT& w) {
    if (auto __v = w.enterSubVector(id.name))
      for (T& v : vector)
        WriteYaml<PropOp>::Do<T, DNAE>(id, v, w);
  }
  template <class T, class S, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, bool>> Do(const PropId& id, std::vector<T>& vector, const S& count,
                                                               StreamT& w) {
    /* libc++ specializes vector<bool> as a bitstream */
    if (auto __v = w.enterSubVector(id.name))
      for (const T& v : vector)
        w.writeBool(nullptr, v);
  }
  static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& w) {
    w.writeUBytes(id.name, buf, count);
  }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::string>> Do(const PropId& id, T& str, StreamT& w) {
    w.writeString(id.name, str);
  }
  static void Do(const PropId& id, std::string& str, atInt32 count, StreamT& w) { w.writeString(id.name, str); }
  template <class T, Endian DNAE>
  static typename std::enable_if_t<std::is_same_v<T, std::wstring>> Do(const PropId& id, T& str, StreamT& w) {
    w.writeWString(id.name, str);
  }
  template <Endian DNAE>
  static void Do(const PropId& id, std::wstring& str, atInt32 count, StreamT& w) {
    w.writeWString(id.name, str);
  }
  static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& w) {}
  static void DoAlign(atInt64 amount, StreamT& w) {}
};
#define __WRITE_YAML_S(type, endian)                                                                                   \
  template <>                                                                                                          \
  template <>                                                                                                          \
  inline void WriteYaml<PropType::None>::Do<type, endian>(const PropId& id, type& var, WriteYaml::StreamT& w)
__WRITE_YAML_S(bool, Endian::Big) { w.writeBool(id.name, var); }
__WRITE_YAML_S(atInt8, Endian::Big) { w.writeByte(id.name, var); }
__WRITE_YAML_S(atUint8, Endian::Big) { w.writeUByte(id.name, var); }
__WRITE_YAML_S(atInt16, Endian::Big) { w.writeInt16(id.name, var); }
__WRITE_YAML_S(atUint16, Endian::Big) { w.writeUint16(id.name, var); }
__WRITE_YAML_S(atInt32, Endian::Big) { w.writeInt32(id.name, var); }
__WRITE_YAML_S(atUint32, Endian::Big) { w.writeUint32(id.name, var); }
__WRITE_YAML_S(atInt64, Endian::Big) { w.writeInt64(id.name, var); }
__WRITE_YAML_S(atUint64, Endian::Big) { w.writeUint64(id.name, var); }
__WRITE_YAML_S(float, Endian::Big) { w.writeFloat(id.name, var); }
__WRITE_YAML_S(double, Endian::Big) { w.writeDouble(id.name, var); }
__WRITE_YAML_S(atVec2f, Endian::Big) { w.writeVec2f(id.name, var); }
__WRITE_YAML_S(atVec2d, Endian::Big) { w.writeVec2d(id.name, var); }
__WRITE_YAML_S(atVec3f, Endian::Big) { w.writeVec3f(id.name, var); }
__WRITE_YAML_S(atVec3d, Endian::Big) { w.writeVec3d(id.name, var); }
__WRITE_YAML_S(atVec4f, Endian::Big) { w.writeVec4f(id.name, var); }
__WRITE_YAML_S(atVec4d, Endian::Big) { w.writeVec4d(id.name, var); }
__WRITE_YAML_S(bool, Endian::Little) { w.writeBool(id.name, var); }
__WRITE_YAML_S(atInt8, Endian::Little) { w.writeByte(id.name, var); }
__WRITE_YAML_S(atUint8, Endian::Little) { w.writeUByte(id.name, var); }
__WRITE_YAML_S(atInt16, Endian::Little) { w.writeInt16(id.name, var); }
__WRITE_YAML_S(atUint16, Endian::Little) { w.writeUint16(id.name, var); }
__WRITE_YAML_S(atInt32, Endian::Little) { w.writeInt32(id.name, var); }
__WRITE_YAML_S(atUint32, Endian::Little) { w.writeUint32(id.name, var); }
__WRITE_YAML_S(atInt64, Endian::Little) { w.writeInt64(id.name, var); }
__WRITE_YAML_S(atUint64, Endian::Little) { w.writeUint64(id.name, var); }
__WRITE_YAML_S(float, Endian::Little) { w.writeFloat(id.name, var); }
__WRITE_YAML_S(double, Endian::Little) { w.writeDouble(id.name, var); }
__WRITE_YAML_S(atVec2f, Endian::Little) { w.writeVec2f(id.name, var); }
__WRITE_YAML_S(atVec2d, Endian::Little) { w.writeVec2d(id.name, var); }
__WRITE_YAML_S(atVec3f, Endian::Little) { w.writeVec3f(id.name, var); }
__WRITE_YAML_S(atVec3d, Endian::Little) { w.writeVec3d(id.name, var); }
__WRITE_YAML_S(atVec4f, Endian::Little) { w.writeVec4f(id.name, var); }
__WRITE_YAML_S(atVec4d, Endian::Little) { w.writeVec4d(id.name, var); }

template <class Op, class T, Endian DNAE>
void __Do(const PropId& id, T& var, typename Op::StreamT& s) {
  Op::template Do<T, DNAE>(id, var, s);
}

template <class Op, class T, Endian DNAE>
void __DoSize(const PropId& id, T& var, typename Op::StreamT& s) {
  Op::template DoSize<T, DNAE>(id, var, s);
}

template <class Op, class T, class S, Endian DNAE>
void __Do(const PropId& id, std::vector<T>& vector, const S& count, typename Op::StreamT& s) {
  Op::template Do<T, S, DNAE>(id, vector, count, s);
}

template <class Op>
void __Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, typename Op::StreamT& s) {
  Op::Do(id, buf, count, s);
}

template <class Op>
void __Do(const PropId& id, std::string& str, atInt32 count, typename Op::StreamT& s) {
  Op::Do(id, str, count, s);
}

template <class Op, Endian DNAE>
void __Do(const PropId& id, std::wstring& str, atInt32 count, typename Op::StreamT& s) {
  Op::template Do<DNAE>(id, str, count, s);
}

template <class Op>
void __DoSeek(atInt64 delta, athena::SeekOrigin whence, typename Op::StreamT& s) {
  Op::DoSeek(delta, whence, s);
}

template <class Op>
void __DoAlign(atInt64 amount, typename Op::StreamT& s) {
  Op::DoAlign(amount, s);
}

template <class T>
void __Read(T& obj, athena::io::IStreamReader& r) {
  __Do<Read<PropType::None>, T, T::DNAEndian>({}, obj, r);
}

template <class T>
void __Write(const T& obj, athena::io::IStreamWriter& w) {
  __Do<Write<PropType::None>, T, T::DNAEndian>({}, const_cast<T&>(obj), w);
}

template <class T>
void __BinarySize(const T& obj, size_t& s) {
  __Do<BinarySize<PropType::None>, T, T::DNAEndian>({}, const_cast<T&>(obj), s);
}

template <class T>
void __PropCount(const T& obj, size_t& s) {
  const_cast<T&>(obj).template Enumerate<PropCount<PropType::None>>(s);
}

template <class T>
void __ReadYaml(T& obj, athena::io::YAMLDocReader& r) {
  obj.template Enumerate<ReadYaml<PropType::None>>(r);
}

template <class T>
void __WriteYaml(const T& obj, athena::io::YAMLDocWriter& w) {
  const_cast<T&>(obj).template Enumerate<WriteYaml<PropType::None>>(w);
}

template <class T>
void __ReadProp(T& obj, athena::io::IStreamReader& r) {
  /* Read root 0xffffffff hash (hashed empty string) */
  T::DNAEndian == Endian::Big ? r.readUint32Big() : r.readUint32Little();
  atInt64 size = T::DNAEndian == Endian::Big ? r.readUint16Big() : r.readUint16Little();
  atInt64 start = r.position();
  __Do<Read<PropType::CRC32>, T, T::DNAEndian>({}, obj, r);
  atInt64 actualRead = r.position() - start;
  if (actualRead != size)
    r.seek(size - actualRead);
}

template <class T>
void __WriteProp(const T& obj, athena::io::IStreamWriter& w) {
  __Do<Write<PropType::CRC32>, T, T::DNAEndian>({}, const_cast<T&>(obj), w);
}

template <class T>
void __BinarySizeProp(const T& obj, size_t& s) {
  __Do<BinarySize<PropType::CRC32>, T, T::DNAEndian>({}, const_cast<T&>(obj), s);
}

template <class T>
void __ReadProp64(T& obj, athena::io::IStreamReader& r) {
  /* Read root 0x0 hash (hashed empty string) */
  T::DNAEndian == Endian::Big ? r.readUint64Big() : r.readUint64Little();
  atInt64 size = T::DNAEndian == Endian::Big ? r.readUint16Big() : r.readUint16Little();
  atInt64 start = r.position();
  __Do<Read<PropType::CRC64>, T, T::DNAEndian>({}, obj, r);
  atInt64 actualRead = r.position() - start;
  if (actualRead != size)
    r.seek(size - actualRead);
}

template <class T>
void __WriteProp64(const T& obj, athena::io::IStreamWriter& w) {
  __Do<Write<PropType::CRC64>, T, T::DNAEndian>({}, const_cast<T&>(obj), w);
}

template <class T>
void __BinarySizeProp64(const T& obj, size_t& s) {
  __Do<BinarySize<PropType::CRC64>, T, T::DNAEndian>({}, const_cast<T&>(obj), s);
}

} // namespace athena::io

#define AT_DECL_DNA_DO                                                                                                 \
  template <class Op, athena::Endian DNAE = DNAEndian, class T>                                                        \
  void Do(const athena::io::PropId& _id, T& var, typename Op::StreamT& s) {                                            \
    athena::io::__Do<Op, T, DNAE>(_id, var, s);                                                                        \
  }                                                                                                                    \
  template <class Op, athena::Endian DNAE = DNAEndian, class T>                                                        \
  void DoSize(const athena::io::PropId& _id, T& var, typename Op::StreamT& s) {                                        \
    athena::io::__DoSize<Op, T, DNAE>(_id, var, s);                                                                    \
  }                                                                                                                    \
  template <class Op, athena::Endian DNAE = DNAEndian, class T, class S>                                               \
  void Do(const athena::io::PropId& _id, std::vector<T>& var, const S& count, typename Op::StreamT& s) {               \
    athena::io::__Do<Op, T, S, DNAE>(_id, var, count, s);                                                              \
  }                                                                                                                    \
  template <class Op>                                                                                                  \
  void Do(const athena::io::PropId& _id, std::unique_ptr<atUint8[]>& buf, size_t count, typename Op::StreamT& s) {     \
    athena::io::__Do<Op>(_id, buf, count, s);                                                                          \
  }                                                                                                                    \
  template <class Op>                                                                                                  \
  void Do(const athena::io::PropId& _id, std::string& str, atInt32 count, typename Op::StreamT& s) {                   \
    athena::io::__Do<Op>(_id, str, count, s);                                                                          \
  }                                                                                                                    \
  template <class Op, athena::Endian DNAE = DNAEndian>                                                                 \
  void Do(const athena::io::PropId& _id, std::wstring& str, atInt32 count, typename Op::StreamT& s) {                  \
    athena::io::__Do<Op, DNAE>(_id, str, count, s);                                                                    \
  }                                                                                                                    \
  template <class Op>                                                                                                  \
  void DoSeek(atInt64 delta, athena::SeekOrigin whence, typename Op::StreamT& s) {                                     \
    athena::io::__DoSeek<Op>(delta, whence, s);                                                                        \
  }                                                                                                                    \
  template <class Op>                                                                                                  \
  void DoAlign(atInt64 amount, typename Op::StreamT& s) {                                                              \
    athena::io::__DoAlign<Op>(amount, s);                                                                              \
  }                                                                                                                    \
  template <class Op>                                                                                                  \
  void Enumerate(typename Op::StreamT& s);                                                                             \
  static const char* DNAType();

#define AT_DECL_DNA                                                                                                    \
  AT_DECL_DNA_DO                                                                                                       \
  void read(athena::io::IStreamReader& r) { athena::io::__Read(*this, r); }                                            \
  void write(athena::io::IStreamWriter& w) const { athena::io::__Write(*this, w); }                                    \
  void binarySize(size_t& s) const { athena::io::__BinarySize(*this, s); }

#define AT_DECL_DNA_YAML                                                                                               \
  AT_DECL_DNA                                                                                                          \
  void read(athena::io::YAMLDocReader& r) { athena::io::__ReadYaml(*this, r); }                                        \
  void write(athena::io::YAMLDocWriter& w) const { athena::io::__WriteYaml(*this, w); }

#define AT_DECL_EXPLICIT_DNA                                                                                           \
  AT_DECL_DNA                                                                                                          \
  Delete __d;

#define AT_DECL_EXPLICIT_DNAV                                                                                          \
  AT_DECL_DNAV                                                                                                         \
  Delete __d;

#define AT_DECL_EXPLICIT_DNAV_NO_TYPE                                                                                  \
  AT_DECL_DNAV_NO_TYPE                                                                                                 \
  Delete __d;

#define AT_DECL_EXPLICIT_DNA_YAML                                                                                      \
  AT_DECL_DNA_YAML                                                                                                     \
  Delete __d;

#define AT_DECL_EXPLICIT_DNA_YAMLV                                                                                     \
  AT_DECL_DNA_YAMLV                                                                                                    \
  Delete __d;

#define AT_DECL_EXPLICIT_DNA_YAMLV_NO_TYPE                                                                             \
  AT_DECL_DNA_YAMLV_NO_TYPE                                                                                            \
  Delete __d;
#define AT_DECL_DNAV                                                                                                   \
  AT_DECL_DNA_DO                                                                                                       \
  void read(athena::io::IStreamReader& r) override { athena::io::__Read(*this, r); }                                   \
  void write(athena::io::IStreamWriter& w) const override { athena::io::__Write(*this, w); }                           \
  void binarySize(size_t& s) const override { athena::io::__BinarySize(*this, s); }                                    \
  const char* DNATypeV() const override { return DNAType(); }

#define AT_DECL_DNAV_NO_TYPE                                                                                           \
  AT_DECL_DNA_DO                                                                                                       \
  void read(athena::io::IStreamReader& r) override { athena::io::__Read(*this, r); }                                   \
  void write(athena::io::IStreamWriter& w) const override { athena::io::__Write(*this, w); }                           \
  void binarySize(size_t& s) const override { athena::io::__BinarySize(*this, s); }

#define AT_DECL_DNA_YAMLV                                                                                              \
  AT_DECL_DNAV                                                                                                         \
  void read(athena::io::YAMLDocReader& r) override { athena::io::__ReadYaml(*this, r); }                               \
  void write(athena::io::YAMLDocWriter& w) const override { athena::io::__WriteYaml(*this, w); }

#define AT_DECL_DNA_YAMLV_NO_TYPE                                                                                      \
  AT_DECL_DNAV_NO_TYPE                                                                                                 \
  void read(athena::io::YAMLDocReader& r) override { athena::io::__ReadYaml(*this, r); }                               \
  void write(athena::io::YAMLDocWriter& w) const override { athena::io::__WriteYaml(*this, w); }

#define AT_SPECIALIZE_DNA(...)                                                                                         \
  template void __VA_ARGS__::Enumerate<athena::io::Read<athena::io::PropType::None>>(                                  \
      athena::io::Read<athena::io::PropType::None>::StreamT & s);                                                      \
  template void __VA_ARGS__::Enumerate<athena::io::Write<athena::io::PropType::None>>(                                 \
      athena::io::Write<athena::io::PropType::None>::StreamT & s);                                                     \
  template void __VA_ARGS__::Enumerate<athena::io::BinarySize<athena::io::PropType::None>>(                            \
      athena::io::BinarySize<athena::io::PropType::None>::StreamT & s);

#define AT_SPECIALIZE_DNA_YAML(...)                                                                                    \
  AT_SPECIALIZE_DNA(__VA_ARGS__)                                                                                       \
  template void __VA_ARGS__::Enumerate<athena::io::ReadYaml<athena::io::PropType::None>>(                              \
      athena::io::ReadYaml<athena::io::PropType::None>::StreamT & s);                                                  \
  template void __VA_ARGS__::Enumerate<athena::io::WriteYaml<athena::io::PropType::None>>(                             \
      athena::io::WriteYaml<athena::io::PropType::None>::StreamT & s);

#define AT_DECL_PROPDNA                                                                                                \
  template <class Op, athena::Endian DNAE = DNAEndian, class T>                                                        \
  void Do(const athena::io::PropId& _id, T& var, typename Op::StreamT& s) {                                            \
    athena::io::__Do<Op, T, DNAE>(_id, var, s);                                                                        \
  }                                                                                                                    \
  template <class Op, athena::Endian DNAE = DNAEndian, class T>                                                        \
  void DoSize(const athena::io::PropId& _id, T& var, typename Op::StreamT& s) {                                        \
    athena::io::__DoSize<Op, T, DNAE>(_id, var, s);                                                                    \
  }                                                                                                                    \
  template <class Op, athena::Endian DNAE = DNAEndian, class T, class S>                                               \
  void Do(const athena::io::PropId& _id, std::vector<T>& var, const S& count, typename Op::StreamT& s) {               \
    athena::io::__Do<Op, T, S, DNAE>(_id, var, count, s);                                                              \
  }                                                                                                                    \
  template <class Op>                                                                                                  \
  void Do(const athena::io::PropId& _id, std::unique_ptr<atUint8[]>& buf, size_t count, typename Op::StreamT& s) {     \
    athena::io::__Do<Op>(_id, buf, count, s);                                                                          \
  }                                                                                                                    \
  template <class Op>                                                                                                  \
  void Do(const athena::io::PropId& _id, std::string& str, atInt32 count, typename Op::StreamT& s) {                   \
    athena::io::__Do<Op>(_id, str, count, s);                                                                          \
  }                                                                                                                    \
  template <class Op, athena::Endian DNAE = DNAEndian>                                                                 \
  void Do(const athena::io::PropId& _id, std::wstring& str, atInt32 count, typename Op::StreamT& s) {                  \
    athena::io::__Do<Op, DNAE>(_id, str, count, s);                                                                    \
  }                                                                                                                    \
  template <class Op>                                                                                                  \
  void DoSeek(atInt64 delta, athena::SeekOrigin whence, typename Op::StreamT& s) {                                     \
    athena::io::__DoSeek<Op>(delta, whence, s);                                                                        \
  }                                                                                                                    \
  template <class Op>                                                                                                  \
  void DoAlign(atInt64 amount, typename Op::StreamT& s) {                                                              \
    athena::io::__DoAlign<Op>(amount, s);                                                                              \
  }                                                                                                                    \
  template <class Op>                                                                                                  \
  void Enumerate(typename Op::StreamT& s);                                                                             \
  template <class Op>                                                                                                  \
  bool Lookup(uint64_t hash, typename Op::StreamT& s);                                                                 \
  static const char* DNAType();                                                                                        \
  void read(athena::io::IStreamReader& r) { athena::io::__Read(*this, r); }                                            \
  void write(athena::io::IStreamWriter& w) const { athena::io::__Write(*this, w); }                                    \
  void binarySize(size_t& s) const { athena::io::__BinarySize(*this, s); }                                             \
  void read(athena::io::YAMLDocReader& r) { athena::io::__ReadYaml(*this, r); }                                        \
  void write(athena::io::YAMLDocWriter& w) const { athena::io::__WriteYaml(*this, w); }                                \
  void readProp(athena::io::IStreamReader& r) { athena::io::__ReadProp(*this, r); }                                    \
  void writeProp(athena::io::IStreamWriter& w) const { athena::io::__WriteProp(*this, w); }                            \
  void binarySizeProp(size_t& s) const { athena::io::__BinarySizeProp(*this, s); }                                     \
  void propCount(size_t& s) const { athena::io::__PropCount(*this, s); }                                               \
  void readProp64(athena::io::IStreamReader& r) { athena::io::__ReadProp64(*this, r); }                                \
  void writeProp64(athena::io::IStreamWriter& w) const { athena::io::__WriteProp64(*this, w); }                        \
  void binarySizeProp64(size_t& s) const { athena::io::__BinarySizeProp64(*this, s); }

#define AT_DECL_EXPLICIT_PROPDNA                                                                                       \
  AT_DECL_PROPDNA                                                                                                      \
  Delete __d;

#define AT_SPECIALIZE_PROPDNA(...)                                                                                     \
  template void __VA_ARGS__::Enumerate<athena::io::Read<athena::io::PropType::None>>(                                  \
      athena::io::Read<athena::io::PropType::None>::StreamT & s);                                                      \
  template void __VA_ARGS__::Enumerate<athena::io::Write<athena::io::PropType::None>>(                                 \
      athena::io::Write<athena::io::PropType::None>::StreamT & s);                                                     \
  template void __VA_ARGS__::Enumerate<athena::io::BinarySize<athena::io::PropType::None>>(                            \
      athena::io::BinarySize<athena::io::PropType::None>::StreamT & s);                                                \
  template void __VA_ARGS__::Enumerate<athena::io::ReadYaml<athena::io::PropType::None>>(                              \
      athena::io::ReadYaml<athena::io::PropType::None>::StreamT & s);                                                  \
  template void __VA_ARGS__::Enumerate<athena::io::WriteYaml<athena::io::PropType::None>>(                             \
      athena::io::WriteYaml<athena::io::PropType::None>::StreamT & s);                                                 \
  template bool __VA_ARGS__::Lookup<athena::io::Read<athena::io::PropType::CRC32>>(                                    \
      uint64_t hash, athena::io::Read<athena::io::PropType::CRC32>::StreamT & s);                                      \
  template void __VA_ARGS__::Enumerate<athena::io::Read<athena::io::PropType::CRC32>>(                                 \
      athena::io::Read<athena::io::PropType::CRC32>::StreamT & s);                                                     \
  template bool __VA_ARGS__::Lookup<athena::io::Write<athena::io::PropType::CRC32>>(                                   \
      uint64_t hash, athena::io::Write<athena::io::PropType::CRC32>::StreamT & s);                                     \
  template void __VA_ARGS__::Enumerate<athena::io::Write<athena::io::PropType::CRC32>>(                                \
      athena::io::Write<athena::io::PropType::CRC32>::StreamT & s);                                                    \
  template bool __VA_ARGS__::Lookup<athena::io::BinarySize<athena::io::PropType::CRC32>>(                              \
      uint64_t hash, athena::io::BinarySize<athena::io::PropType::CRC32>::StreamT & s);                                \
  template void __VA_ARGS__::Enumerate<athena::io::BinarySize<athena::io::PropType::CRC32>>(                           \
      athena::io::BinarySize<athena::io::PropType::CRC32>::StreamT & s);                                               \
  template bool __VA_ARGS__::Lookup<athena::io::PropCount<athena::io::PropType::CRC32>>(                               \
      uint64_t hash, athena::io::PropCount<athena::io::PropType::CRC32>::StreamT & s);                                 \
  template void __VA_ARGS__::Enumerate<athena::io::PropCount<athena::io::PropType::CRC32>>(                            \
      athena::io::PropCount<athena::io::PropType::CRC32>::StreamT & s);                                                \
  template bool __VA_ARGS__::Lookup<athena::io::ReadYaml<athena::io::PropType::CRC32>>(                                \
      uint64_t hash, athena::io::ReadYaml<athena::io::PropType::CRC32>::StreamT & s);                                  \
  template void __VA_ARGS__::Enumerate<athena::io::ReadYaml<athena::io::PropType::CRC32>>(                             \
      athena::io::ReadYaml<athena::io::PropType::CRC32>::StreamT & s);                                                 \
  template bool __VA_ARGS__::Lookup<athena::io::WriteYaml<athena::io::PropType::CRC32>>(                               \
      uint64_t hash, athena::io::WriteYaml<athena::io::PropType::CRC32>::StreamT & s);                                 \
  template void __VA_ARGS__::Enumerate<athena::io::WriteYaml<athena::io::PropType::CRC32>>(                            \
      athena::io::WriteYaml<athena::io::PropType::CRC32>::StreamT & s);                                                \
  template bool __VA_ARGS__::Lookup<athena::io::Read<athena::io::PropType::CRC64>>(                                    \
      uint64_t hash, athena::io::Read<athena::io::PropType::CRC64>::StreamT & s);                                      \
  template void __VA_ARGS__::Enumerate<athena::io::Read<athena::io::PropType::CRC64>>(                                 \
      athena::io::Read<athena::io::PropType::CRC64>::StreamT & s);                                                     \
  template bool __VA_ARGS__::Lookup<athena::io::Write<athena::io::PropType::CRC64>>(                                   \
      uint64_t hash, athena::io::Write<athena::io::PropType::CRC64>::StreamT & s);                                     \
  template void __VA_ARGS__::Enumerate<athena::io::Write<athena::io::PropType::CRC64>>(                                \
      athena::io::Write<athena::io::PropType::CRC64>::StreamT & s);                                                    \
  template bool __VA_ARGS__::Lookup<athena::io::BinarySize<athena::io::PropType::CRC64>>(                              \
      uint64_t hash, athena::io::BinarySize<athena::io::PropType::CRC64>::StreamT & s);                                \
  template void __VA_ARGS__::Enumerate<athena::io::BinarySize<athena::io::PropType::CRC64>>(                           \
      athena::io::BinarySize<athena::io::PropType::CRC64>::StreamT & s);                                               \
  template bool __VA_ARGS__::Lookup<athena::io::PropCount<athena::io::PropType::CRC64>>(                               \
      uint64_t hash, athena::io::PropCount<athena::io::PropType::CRC64>::StreamT & s);                                 \
  template void __VA_ARGS__::Enumerate<athena::io::PropCount<athena::io::PropType::CRC64>>(                            \
      athena::io::PropCount<athena::io::PropType::CRC64>::StreamT & s);                                                \
  template bool __VA_ARGS__::Lookup<athena::io::ReadYaml<athena::io::PropType::CRC64>>(                                \
      uint64_t hash, athena::io::ReadYaml<athena::io::PropType::CRC64>::StreamT & s);                                  \
  template void __VA_ARGS__::Enumerate<athena::io::ReadYaml<athena::io::PropType::CRC64>>(                             \
      athena::io::ReadYaml<athena::io::PropType::CRC64>::StreamT & s);                                                 \
  template bool __VA_ARGS__::Lookup<athena::io::WriteYaml<athena::io::PropType::CRC64>>(                               \
      uint64_t hash, athena::io::WriteYaml<athena::io::PropType::CRC64>::StreamT & s);                                 \
  template void __VA_ARGS__::Enumerate<athena::io::WriteYaml<athena::io::PropType::CRC64>>(                            \
      athena::io::WriteYaml<athena::io::PropType::CRC64>::StreamT & s);

#define AT_SUBDECL_DNA                                                                                                 \
  void _read(athena::io::IStreamReader& r);                                                                            \
  void _write(athena::io::IStreamWriter& w) const;                                                                     \
  void _binarySize(size_t& s) const;                                                                                   \
  void _read(athena::io::YAMLDocReader& r);                                                                            \
  void _write(athena::io::YAMLDocWriter& w) const;

#define AT_SUBDECL_DNA_YAML                                                                                            \
  AT_SUBDECL_DNA                                                                                                       \
  void _read(athena::io::YAMLDocReader& r);                                                                            \
  void _write(athena::io::YAMLDocWriter& w) const;

#define AT_SUBSPECIALIZE_DNA(...)                                                                                      \
  template <>                                                                                                          \
  template <>                                                                                                          \
  void __VA_ARGS__::Enumerate<athena::io::DNA<athena::Big>::BinarySize>(typename BinarySize::StreamT & s) {            \
    _binarySize(s);                                                                                                    \
  }                                                                                                                    \
  template <>                                                                                                          \
  template <>                                                                                                          \
  void __VA_ARGS__::Enumerate<athena::io::DNA<athena::Big>::Read>(typename Read::StreamT & r) {                        \
    _read(r);                                                                                                          \
  }                                                                                                                    \
  template <>                                                                                                          \
  template <>                                                                                                          \
  void __VA_ARGS__::Enumerate<athena::io::DNA<athena::Big>::Write>(typename Write::StreamT & w) {                      \
    _write(w);                                                                                                         \
  }

#define AT_SUBSPECIALIZE_DNA_YAML(...)                                                                                 \
  template <>                                                                                                          \
  template <>                                                                                                          \
  void __VA_ARGS__::Enumerate<athena::io::DNA<athena::Big>::ReadYaml>(typename ReadYaml::StreamT & r) {                \
    _read(r);                                                                                                          \
  }                                                                                                                    \
  template <>                                                                                                          \
  template <>                                                                                                          \
  void __VA_ARGS__::Enumerate<athena::io::DNA<athena::Big>::WriteYaml>(typename WriteYaml::StreamT & w) {              \
    _write(w);                                                                                                         \
  }                                                                                                                    \
  AT_SUBSPECIALIZE_DNA(__VA_ARGS__)
