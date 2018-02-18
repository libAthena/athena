#ifndef AT_OP_HPP
#define AT_OP_HPP

#include "IStreamReader.hpp"
#include "IStreamWriter.hpp"
#include "DNAYaml.hpp"
#include "ChecksumsLiterals.hpp"

namespace athena::io
{

struct PropId
{
    const char* name = nullptr;
    uint32_t rcrc32 = 0xffffffff;
    uint64_t crc64 = 0x0;
    template <class T> constexpr T opget() const;
    constexpr PropId() = default;
    constexpr PropId(const char* name, uint32_t rcrc32, uint64_t crc64)
    : name(name), rcrc32(rcrc32), crc64(crc64) {}
    constexpr PropId(const char* name)
    : name(name),
      rcrc32(athena::checksums::literals::rcrc32_rec(0xFFFFFFFF, name)),
      crc64(athena::checksums::literals::crc64_rec(0xFFFFFFFFFFFFFFFF, name)) {}
    constexpr PropId(const char* name, uint32_t rcrc32)
    : name(name),
      rcrc32(rcrc32),
      crc64(athena::checksums::literals::crc64_rec(0xFFFFFFFFFFFFFFFF, name)) {}
};

template <>
constexpr uint32_t PropId::opget<uint32_t>() const { return rcrc32; }

template <>
constexpr uint64_t PropId::opget<uint64_t>() const { return crc64; }

namespace literals
{
constexpr PropId operator "" _propid(const char* s, size_t len)
{
    return {s};
}
}

#define AT_PROP_CASE(...) case athena::io::PropId(__VA_ARGS__).opget<typename Op::PropT>()

#if defined(__atdna__)
#  define AT_OVERRIDE_RCRC32(rcrc32) __attribute__((annotate("rcrc32=" #rcrc32)))
#else
#  define AT_OVERRIDE_RCRC32(rcrc32)
#endif

enum class PropType
{
    None,
    CRC32,
    CRC64
};

template <class T>
static inline constexpr bool __IsPODType()
{
    return std::is_arithmetic<T>::value ||
           std::is_same<T, atVec2f>::value ||
           std::is_same<T, atVec3f>::value ||
           std::is_same<T, atVec4f>::value ||
           std::is_same<T, atVec2d>::value ||
           std::is_same<T, atVec3d>::value ||
           std::is_same<T, atVec4d>::value;
}

template <PropType PropOp>
struct BinarySize
{
    using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
    using StreamT = size_t;
    template <class T, Endian DNAE>
    static typename std::enable_if_t<std::is_enum_v<T>>
    Do(const PropId& id, T& var, StreamT& s)
    {
        if (PropOp != PropType::None)
        {
            /* Accessed via Enumerate, header */
            s += 6;
        }
        using PODType = std::underlying_type_t<T>;
        BinarySize<PropType::None>::Do<PODType, DNAE>(id, *reinterpret_cast<PODType*>(&var), s);
    }
    template <class T, Endian DNAE>
    static typename std::enable_if_t<__IsPODType<T>()>
    Do(const PropId& id, T& var, StreamT& s)
    {
        if (PropOp != PropType::None)
        {
            /* Accessed via Enumerate, header */
            s += 6;
        }
        BinarySize<PropType::None>::Do<T, DNAE>(id, var, s);
    }
    template <class T, Endian DNAE>
    static typename std::enable_if_t<!std::is_enum_v<T> && !__IsPODType<T>()>
    Do(const PropId& id, T& var, StreamT& s)
    {
        if (PropOp != PropType::None)
        {
            /* Accessed via Enumerate, header */
            s += 6;
            var.template Enumerate<BinarySize<PropOp>>(s);
        }
        else
            var.template Enumerate<BinarySize>(s);
    }
    template <class T, Endian DNAE>
    static void Do(const PropId& id, std::vector<T>& vector, size_t count, StreamT& s)
    {
        for (T& v : vector)
            BinarySize<PropOp>::Do<T, DNAE>(id, v, s);
    }
    static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& s)
    {
        if (buf)
            s += count;
    }
    static void Do(const PropId& id, std::string& str, StreamT& s)
    {
        s += str.size() + 1;
    }
    static void Do(const PropId& id, std::string& str, size_t count, StreamT& s)
    {
        s += count;
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, StreamT& s)
    {
        s += str.size() * 2 + 2;
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, size_t count, StreamT& s)
    {
        s += count * 2;
    }
    static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& s)
    {
        switch (whence)
        {
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
    static void DoAlign(atInt64 amount, StreamT& s)
    {
        s = (s + amount-1) / amount * amount;
    }
};
template <> template <> inline void BinarySize<PropType::None>::Do<bool, Endian::Big>(const PropId& id, bool& var, BinarySize::StreamT& s) { s += 1; }
template <> template <> inline void BinarySize<PropType::None>::Do<atInt8, Endian::Big>(const PropId& id, atInt8& var, BinarySize::StreamT& s) { s += 1; }
template <> template <> inline void BinarySize<PropType::None>::Do<atUint8, Endian::Big>(const PropId& id, atUint8& var, BinarySize::StreamT& s) { s += 1; }
template <> template <> inline void BinarySize<PropType::None>::Do<atInt16, Endian::Big>(const PropId& id, atInt16& var, BinarySize::StreamT& s) { s += 2; }
template <> template <> inline void BinarySize<PropType::None>::Do<atUint16, Endian::Big>(const PropId& id, atUint16& var, BinarySize::StreamT& s) { s += 2; }
template <> template <> inline void BinarySize<PropType::None>::Do<atInt32, Endian::Big>(const PropId& id, atInt32& var, BinarySize::StreamT& s) { s += 4; }
template <> template <> inline void BinarySize<PropType::None>::Do<atUint32, Endian::Big>(const PropId& id, atUint32& var, BinarySize::StreamT& s) { s += 4; }
template <> template <> inline void BinarySize<PropType::None>::Do<atInt64, Endian::Big>(const PropId& id, atInt64& var, BinarySize::StreamT& s) { s += 8; }
template <> template <> inline void BinarySize<PropType::None>::Do<atUint64, Endian::Big>(const PropId& id, atUint64& var, BinarySize::StreamT& s) { s += 8; }
template <> template <> inline void BinarySize<PropType::None>::Do<float, Endian::Big>(const PropId& id, float& var, BinarySize::StreamT& s) { s += 4; }
template <> template <> inline void BinarySize<PropType::None>::Do<double, Endian::Big>(const PropId& id, double& var, BinarySize::StreamT& s) { s += 8; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec2f, Endian::Big>(const PropId& id, atVec2f& var, BinarySize::StreamT& s) { s += 8; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec2d, Endian::Big>(const PropId& id, atVec2d& var, BinarySize::StreamT& s) { s += 16; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec3f, Endian::Big>(const PropId& id, atVec3f& var, BinarySize::StreamT& s) { s += 12; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec3d, Endian::Big>(const PropId& id, atVec3d& var, BinarySize::StreamT& s) { s += 24; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec4f, Endian::Big>(const PropId& id, atVec4f& var, BinarySize::StreamT& s) { s += 16; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec4d, Endian::Big>(const PropId& id, atVec4d& var, BinarySize::StreamT& s) { s += 32; }
template <> template <> inline void BinarySize<PropType::None>::Do<bool, Endian::Little>(const PropId& id, bool& var, BinarySize::StreamT& s) { s += 1; }
template <> template <> inline void BinarySize<PropType::None>::Do<atInt8, Endian::Little>(const PropId& id, atInt8& var, BinarySize::StreamT& s) { s += 1; }
template <> template <> inline void BinarySize<PropType::None>::Do<atUint8, Endian::Little>(const PropId& id, atUint8& var, BinarySize::StreamT& s) { s += 1; }
template <> template <> inline void BinarySize<PropType::None>::Do<atInt16, Endian::Little>(const PropId& id, atInt16& var, BinarySize::StreamT& s) { s += 2; }
template <> template <> inline void BinarySize<PropType::None>::Do<atUint16, Endian::Little>(const PropId& id, atUint16& var, BinarySize::StreamT& s) { s += 2; }
template <> template <> inline void BinarySize<PropType::None>::Do<atInt32, Endian::Little>(const PropId& id, atInt32& var, BinarySize::StreamT& s) { s += 4; }
template <> template <> inline void BinarySize<PropType::None>::Do<atUint32, Endian::Little>(const PropId& id, atUint32& var, BinarySize::StreamT& s) { s += 4; }
template <> template <> inline void BinarySize<PropType::None>::Do<atInt64, Endian::Little>(const PropId& id, atInt64& var, BinarySize::StreamT& s) { s += 8; }
template <> template <> inline void BinarySize<PropType::None>::Do<atUint64, Endian::Little>(const PropId& id, atUint64& var, BinarySize::StreamT& s) { s += 8; }
template <> template <> inline void BinarySize<PropType::None>::Do<float, Endian::Little>(const PropId& id, float& var, BinarySize::StreamT& s) { s += 4; }
template <> template <> inline void BinarySize<PropType::None>::Do<double, Endian::Little>(const PropId& id, double& var, BinarySize::StreamT& s) { s += 8; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec2f, Endian::Little>(const PropId& id, atVec2f& var, BinarySize::StreamT& s) { s += 8; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec2d, Endian::Little>(const PropId& id, atVec2d& var, BinarySize::StreamT& s) { s += 16; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec3f, Endian::Little>(const PropId& id, atVec3f& var, BinarySize::StreamT& s) { s += 12; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec3d, Endian::Little>(const PropId& id, atVec3d& var, BinarySize::StreamT& s) { s += 24; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec4f, Endian::Little>(const PropId& id, atVec4f& var, BinarySize::StreamT& s) { s += 16; }
template <> template <> inline void BinarySize<PropType::None>::Do<atVec4d, Endian::Little>(const PropId& id, atVec4d& var, BinarySize::StreamT& s) { s += 32; }

template <PropType PropOp>
struct PropCount
{
    using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
    using StreamT = size_t;
    template <class T, Endian DNAE>
    static void Do(const PropId& id, T& var, StreamT& s)
    {
        /* Only reports one level of properties */
        s += 1;
    }
    template <class T, Endian DNAE>
    static void Do(const PropId& id, std::vector<T>& vector, size_t count, StreamT& s)
    {
        /* Only reports one level of properties */
        s += 1;
    }
    static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& s)
    {
        /* Only reports one level of properties */
        s += 1;
    }
    static void Do(const PropId& id, std::string& str, StreamT& s)
    {
        /* Only reports one level of properties */
        s += 1;
    }
    static void Do(const PropId& id, std::string& str, size_t count, StreamT& s)
    {
        /* Only reports one level of properties */
        s += 1;
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, StreamT& s)
    {
        /* Only reports one level of properties */
        s += 1;
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, size_t count, StreamT& s)
    {
        /* Only reports one level of properties */
        s += 1;
    }
    static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& s) {}
    static void DoAlign(atInt64 amount, StreamT& s) {}
};

template <PropType PropOp>
struct Read
{
    using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
    using StreamT = IStreamReader;
    template <class T, Endian DNAE>
    static typename std::enable_if_t<std::is_enum_v<T>>
    Do(const PropId& id, T& var, StreamT& r)
    {
        using PODType = std::underlying_type_t<T>;
        Read<PropType::None>::Do<PODType, DNAE>(id, *reinterpret_cast<PODType*>(&var), r);
    }
    template <class T, Endian DNAE>
    static typename std::enable_if_t<__IsPODType<T>()>
    Do(const PropId& id, T& var, StreamT& r)
    {
        Read<PropType::None>::Do<T, DNAE>(id, var, r);
    }
    template <class T, Endian DNAE>
    static typename std::enable_if_t<!std::is_enum_v<T> && !__IsPODType<T>()>
    Do(const PropId& id, T& var, StreamT& r)
    {
        if (PropOp != PropType::None)
        {
            /* Accessed via Lookup, no header */
            atUint16 propCount = T::DNAEndian == Endian::Big ? r.readUint16Big() : r.readUint16Little();
            for (atUint32 i = 0; i < propCount; ++i)
            {
                atUint64 hash;
                if (PropOp == PropType::CRC64)
                    hash = T::DNAEndian == Endian::Big ? r.readUint64Big() : r.readUint64Little();
                else
                    hash = T::DNAEndian == Endian::Big ? r.readUint32Big() : r.readUint32Little();
                atInt64 size = T::DNAEndian == Endian::Big ? r.readUint16Big() : r.readUint16Little();
                atInt64 start = r.position();
                var.template Lookup<Read<PropOp>>(hash, r);
                atInt64 actualRead = r.position() - start;
                if (actualRead != size)
                    r.seek(size - actualRead);
            }
        }
        else
            var.template Enumerate<Read<PropOp>>(r);
    }
    template <class T, Endian DNAE>
    static void Do(const PropId& id, std::vector<T>& vector, size_t count, StreamT& r)
    {
        vector.clear();
        vector.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            vector.emplace_back();
            Read<PropOp>::Do<T, DNAE>(id, vector.back(), r);
        }
    }
    static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& r)
    {
        buf.reset(new atUint8[count]);
        r.readUBytesToBuf(buf.get(), count);
    }
    static void Do(const PropId& id, std::string& str, StreamT& r)
    {
        str = r.readString();
    }
    static void Do(const PropId& id, std::string& str, size_t count, StreamT& r)
    {
        str = r.readString(count);
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, StreamT& r)
    {
        Read<PropType::None>::Do<DNAE>(id, str, r);
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, size_t count, StreamT& r)
    {
        Read<PropType::None>::Do<DNAE>(id, str, count, r);
    }
    static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& r)
    {
        r.seek(amount, whence);
    }
    static void DoAlign(atInt64 amount, StreamT& r)
    {
        r.seek((r.position() + amount-1) / amount * amount, athena::Begin);
    }
};
template <> template <> inline void Read<PropType::None>::Do<bool, Endian::Big>(const PropId& id, bool& var, Read::StreamT& r) { var = r.readBool(); }
template <> template <> inline void Read<PropType::None>::Do<atInt8, Endian::Big>(const PropId& id, atInt8& var, Read::StreamT& r) { var = r.readByte(); }
template <> template <> inline void Read<PropType::None>::Do<atUint8, Endian::Big>(const PropId& id, atUint8& var, Read::StreamT& r) { var = r.readUByte(); }
template <> template <> inline void Read<PropType::None>::Do<atInt16, Endian::Big>(const PropId& id, atInt16& var, Read::StreamT& r) { var = r.readInt16Big(); }
template <> template <> inline void Read<PropType::None>::Do<atUint16, Endian::Big>(const PropId& id, atUint16& var, Read::StreamT& r) { var = r.readUint16Big(); }
template <> template <> inline void Read<PropType::None>::Do<atInt32, Endian::Big>(const PropId& id, atInt32& var, Read::StreamT& r) { var = r.readInt32Big(); }
template <> template <> inline void Read<PropType::None>::Do<atUint32, Endian::Big>(const PropId& id, atUint32& var, Read::StreamT& r) { var = r.readUint32Big(); }
template <> template <> inline void Read<PropType::None>::Do<atInt64, Endian::Big>(const PropId& id, atInt64& var, Read::StreamT& r) { var = r.readInt64Big(); }
template <> template <> inline void Read<PropType::None>::Do<atUint64, Endian::Big>(const PropId& id, atUint64& var, Read::StreamT& r) { var = r.readUint64Big(); }
template <> template <> inline void Read<PropType::None>::Do<float, Endian::Big>(const PropId& id, float& var, Read::StreamT& r) { var = r.readFloatBig(); }
template <> template <> inline void Read<PropType::None>::Do<double, Endian::Big>(const PropId& id, double& var, Read::StreamT& r) { var = r.readDoubleBig(); }
template <> template <> inline void Read<PropType::None>::Do<atVec2f, Endian::Big>(const PropId& id, atVec2f& var, Read::StreamT& r) { var = r.readVec2fBig(); }
template <> template <> inline void Read<PropType::None>::Do<atVec2d, Endian::Big>(const PropId& id, atVec2d& var, Read::StreamT& r) { var = r.readVec2dBig(); }
template <> template <> inline void Read<PropType::None>::Do<atVec3f, Endian::Big>(const PropId& id, atVec3f& var, Read::StreamT& r) { var = r.readVec3fBig(); }
template <> template <> inline void Read<PropType::None>::Do<atVec3d, Endian::Big>(const PropId& id, atVec3d& var, Read::StreamT& r) { var = r.readVec3dBig(); }
template <> template <> inline void Read<PropType::None>::Do<atVec4f, Endian::Big>(const PropId& id, atVec4f& var, Read::StreamT& r) { var = r.readVec4fBig(); }
template <> template <> inline void Read<PropType::None>::Do<atVec4d, Endian::Big>(const PropId& id, atVec4d& var, Read::StreamT& r) { var = r.readVec4dBig(); }
template <> template <> inline void Read<PropType::None>::Do<Endian::Big>(const PropId& id, std::wstring& str, StreamT& r) { str = r.readWStringBig(); }
template <> template <> inline void Read<PropType::None>::Do<Endian::Big>(const PropId& id, std::wstring& str, size_t count, StreamT& r) { str = r.readWStringBig(count); }
template <> template <> inline void Read<PropType::None>::Do<bool, Endian::Little>(const PropId& id, bool& var, Read::StreamT& r) { var = r.readBool(); }
template <> template <> inline void Read<PropType::None>::Do<atInt8, Endian::Little>(const PropId& id, atInt8& var, Read::StreamT& r) { var = r.readByte(); }
template <> template <> inline void Read<PropType::None>::Do<atUint8, Endian::Little>(const PropId& id, atUint8& var, Read::StreamT& r) { var = r.readUByte(); }
template <> template <> inline void Read<PropType::None>::Do<atInt16, Endian::Little>(const PropId& id, atInt16& var, Read::StreamT& r) { var = r.readInt16Little(); }
template <> template <> inline void Read<PropType::None>::Do<atUint16, Endian::Little>(const PropId& id, atUint16& var, Read::StreamT& r) { var = r.readUint16Little(); }
template <> template <> inline void Read<PropType::None>::Do<atInt32, Endian::Little>(const PropId& id, atInt32& var, Read::StreamT& r) { var = r.readInt32Little(); }
template <> template <> inline void Read<PropType::None>::Do<atUint32, Endian::Little>(const PropId& id, atUint32& var, Read::StreamT& r) { var = r.readUint32Little(); }
template <> template <> inline void Read<PropType::None>::Do<atInt64, Endian::Little>(const PropId& id, atInt64& var, Read::StreamT& r) { var = r.readInt64Little(); }
template <> template <> inline void Read<PropType::None>::Do<atUint64, Endian::Little>(const PropId& id, atUint64& var, Read::StreamT& r) { var = r.readUint64Little(); }
template <> template <> inline void Read<PropType::None>::Do<float, Endian::Little>(const PropId& id, float& var, Read::StreamT& r) { var = r.readFloatLittle(); }
template <> template <> inline void Read<PropType::None>::Do<double, Endian::Little>(const PropId& id, double& var, Read::StreamT& r) { var = r.readDoubleLittle(); }
template <> template <> inline void Read<PropType::None>::Do<atVec2f, Endian::Little>(const PropId& id, atVec2f& var, Read::StreamT& r) { var = r.readVec2fLittle(); }
template <> template <> inline void Read<PropType::None>::Do<atVec2d, Endian::Little>(const PropId& id, atVec2d& var, Read::StreamT& r) { var = r.readVec2dLittle(); }
template <> template <> inline void Read<PropType::None>::Do<atVec3f, Endian::Little>(const PropId& id, atVec3f& var, Read::StreamT& r) { var = r.readVec3fLittle(); }
template <> template <> inline void Read<PropType::None>::Do<atVec3d, Endian::Little>(const PropId& id, atVec3d& var, Read::StreamT& r) { var = r.readVec3dLittle(); }
template <> template <> inline void Read<PropType::None>::Do<atVec4f, Endian::Little>(const PropId& id, atVec4f& var, Read::StreamT& r) { var = r.readVec4fLittle(); }
template <> template <> inline void Read<PropType::None>::Do<atVec4d, Endian::Little>(const PropId& id, atVec4d& var, Read::StreamT& r) { var = r.readVec4dLittle(); }
template <> template <> inline void Read<PropType::None>::Do<Endian::Little>(const PropId& id, std::wstring& str, StreamT& r) { str = r.readWStringLittle(); }
template <> template <> inline void Read<PropType::None>::Do<Endian::Little>(const PropId& id, std::wstring& str, size_t count, StreamT& r) { str = r.readWStringLittle(count); }

template <PropType PropOp>
struct Write
{
    using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
    using StreamT = IStreamWriter;
    template <class T, Endian DNAE>
    static typename std::enable_if_t<std::is_enum_v<T>>
    Do(const PropId& id, T& var, StreamT& w)
    {
        if (PropOp != PropType::None)
        {
            /* Accessed via Enumerate, header */
            if (PropOp == PropType::CRC64)
                DNAE == Endian::Big ? w.writeUint64Big(id.crc64) : w.writeUint64Little(id.crc64);
            else
                DNAE == Endian::Big ? w.writeUint32Big(id.rcrc32) : w.writeUint32Little(id.rcrc32);
            size_t binarySize = 0;
            BinarySize<PropType::None>::Do<T, DNAE>(id, var, binarySize);
            DNAE == Endian::Big ? w.writeUint16Big(atUint16(binarySize)) : w.writeUint16Little(atUint16(binarySize));
        }
        using PODType = std::underlying_type_t<T>;
        Write<PropType::None>::Do<PODType, DNAE>(id, *reinterpret_cast<PODType*>(&var), w);
    }
    template <class T, Endian DNAE>
    static typename std::enable_if_t<__IsPODType<T>()>
    Do(const PropId& id, T& var, StreamT& w)
    {
        if (PropOp != PropType::None)
        {
            /* Accessed via Enumerate, header */
            if (PropOp == PropType::CRC64)
                DNAE == Endian::Big ? w.writeUint64Big(id.crc64) : w.writeUint64Little(id.crc64);
            else
                DNAE == Endian::Big ? w.writeUint32Big(id.rcrc32) : w.writeUint32Little(id.rcrc32);
            size_t binarySize = 0;
            BinarySize<PropType::None>::Do<T, DNAE>(id, var, binarySize);
            DNAE == Endian::Big ? w.writeUint16Big(atUint16(binarySize)) : w.writeUint16Little(atUint16(binarySize));
        }
        Write<PropType::None>::Do<T, DNAE>(id, var, w);
    }
    template <class T, Endian DNAE>
    static typename std::enable_if_t<!std::is_enum_v<T> && !__IsPODType<T>()>
    Do(const PropId& id, T& var, StreamT& w)
    {
        if (PropOp != PropType::None)
        {
            /* Accessed via Enumerate, header */
            if (PropOp == PropType::CRC64)
                T::DNAEndian == Endian::Big ? w.writeUint64Big(id.crc64) : w.writeUint64Little(id.crc64);
            else
                T::DNAEndian == Endian::Big ? w.writeUint32Big(id.rcrc32) : w.writeUint32Little(id.rcrc32);
            size_t binarySize = 0;
            var.template Enumerate<BinarySize<PropOp>>(binarySize);
            T::DNAEndian == Endian::Big ? w.writeUint16Big(atUint16(binarySize)) : w.writeUint16Little(atUint16(binarySize));

            size_t propCount = 0;
            var.template Enumerate<PropCount<PropOp>>(propCount);
            T::DNAEndian == Endian::Big ? w.writeUint16Big(atUint16(propCount)) : w.writeUint16Little(atUint16(propCount));
            var.template Enumerate<Write<PropOp>>(w);
        }
        else
            var.template Enumerate<Write<PropOp>>(w);
    }
    template <class T, Endian DNAE>
    static void Do(const PropId& id, std::vector<T>& vector, size_t count, StreamT& w)
    {
        for (T& v : vector)
            Write<PropOp>::Do<T, DNAE>(id, v, w);
    }
    static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& w)
    {
        if (buf)
            w.writeUBytes(buf.get(), count);
    }
    static void Do(const PropId& id, std::string& str, StreamT& w)
    {
        w.writeString(str);
    }
    static void Do(const PropId& id, std::string& str, size_t count, StreamT& w)
    {
        w.writeString(str, count);
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, StreamT& w)
    {
        Write<PropType::None>::Do<DNAE>(id, str, w);
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, size_t count, StreamT& w)
    {
        Write<PropType::None>::Do<DNAE>(id, str, count, w);
    }
    static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& w)
    {
        w.seek(amount, whence);
    }
    static void DoAlign(atInt64 amount, StreamT& w)
    {
        w.writeZeroTo((w.position() + amount-1) / amount * amount);
    }
};
template <> template <> inline void Write<PropType::None>::Do<bool, Endian::Big>(const PropId& id, bool& var, Write::StreamT& w) { w.writeBool(var); }
template <> template <> inline void Write<PropType::None>::Do<atInt8, Endian::Big>(const PropId& id, atInt8& var, Write::StreamT& w) { w.writeByte(var); }
template <> template <> inline void Write<PropType::None>::Do<atUint8, Endian::Big>(const PropId& id, atUint8& var, Write::StreamT& w) { w.writeUByte(var); }
template <> template <> inline void Write<PropType::None>::Do<atInt16, Endian::Big>(const PropId& id, atInt16& var, Write::StreamT& w) { w.writeInt16Big(var); }
template <> template <> inline void Write<PropType::None>::Do<atUint16, Endian::Big>(const PropId& id, atUint16& var, Write::StreamT& w) { w.writeUint16Big(var); }
template <> template <> inline void Write<PropType::None>::Do<atInt32, Endian::Big>(const PropId& id, atInt32& var, Write::StreamT& w) { w.writeInt32Big(var); }
template <> template <> inline void Write<PropType::None>::Do<atUint32, Endian::Big>(const PropId& id, atUint32& var, Write::StreamT& w) { w.writeUint32Big(var); }
template <> template <> inline void Write<PropType::None>::Do<atInt64, Endian::Big>(const PropId& id, atInt64& var, Write::StreamT& w) { w.writeInt64Big(var); }
template <> template <> inline void Write<PropType::None>::Do<atUint64, Endian::Big>(const PropId& id, atUint64& var, Write::StreamT& w) { w.writeUint64Big(var); }
template <> template <> inline void Write<PropType::None>::Do<float, Endian::Big>(const PropId& id, float& var, Write::StreamT& w) { w.writeFloatBig(var); }
template <> template <> inline void Write<PropType::None>::Do<double, Endian::Big>(const PropId& id, double& var, Write::StreamT& w) { w.writeDoubleBig(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec2f, Endian::Big>(const PropId& id, atVec2f& var, Write::StreamT& w) { w.writeVec2fBig(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec2d, Endian::Big>(const PropId& id, atVec2d& var, Write::StreamT& w) { w.writeVec2dBig(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec3f, Endian::Big>(const PropId& id, atVec3f& var, Write::StreamT& w) { w.writeVec3fBig(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec3d, Endian::Big>(const PropId& id, atVec3d& var, Write::StreamT& w) { w.writeVec3dBig(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec4f, Endian::Big>(const PropId& id, atVec4f& var, Write::StreamT& w) { w.writeVec4fBig(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec4d, Endian::Big>(const PropId& id, atVec4d& var, Write::StreamT& w) { w.writeVec4dBig(var); }
template <> template <> inline void Write<PropType::None>::Do<Endian::Big>(const PropId& id, std::wstring& str, StreamT& w) { w.writeWStringBig(str); }
template <> template <> inline void Write<PropType::None>::Do<Endian::Big>(const PropId& id, std::wstring& str, size_t count, StreamT& w) { w.writeWStringBig(str, count); }
template <> template <> inline void Write<PropType::None>::Do<bool, Endian::Little>(const PropId& id, bool& var, Write::StreamT& w) { w.writeBool(var); }
template <> template <> inline void Write<PropType::None>::Do<atInt8, Endian::Little>(const PropId& id, atInt8& var, Write::StreamT& w) { w.writeByte(var); }
template <> template <> inline void Write<PropType::None>::Do<atUint8, Endian::Little>(const PropId& id, atUint8& var, Write::StreamT& w) { w.writeUByte(var); }
template <> template <> inline void Write<PropType::None>::Do<atInt16, Endian::Little>(const PropId& id, atInt16& var, Write::StreamT& w) { w.writeInt16Little(var); }
template <> template <> inline void Write<PropType::None>::Do<atUint16, Endian::Little>(const PropId& id, atUint16& var, Write::StreamT& w) { w.writeUint16Little(var); }
template <> template <> inline void Write<PropType::None>::Do<atInt32, Endian::Little>(const PropId& id, atInt32& var, Write::StreamT& w) { w.writeInt32Little(var); }
template <> template <> inline void Write<PropType::None>::Do<atUint32, Endian::Little>(const PropId& id, atUint32& var, Write::StreamT& w) { w.writeUint32Little(var); }
template <> template <> inline void Write<PropType::None>::Do<atInt64, Endian::Little>(const PropId& id, atInt64& var, Write::StreamT& w) { w.writeInt64Little(var); }
template <> template <> inline void Write<PropType::None>::Do<atUint64, Endian::Little>(const PropId& id, atUint64& var, Write::StreamT& w) { w.writeUint64Little(var); }
template <> template <> inline void Write<PropType::None>::Do<float, Endian::Little>(const PropId& id, float& var, Write::StreamT& w) { w.writeFloatLittle(var); }
template <> template <> inline void Write<PropType::None>::Do<double, Endian::Little>(const PropId& id, double& var, Write::StreamT& w) { w.writeDoubleLittle(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec2f, Endian::Little>(const PropId& id, atVec2f& var, Write::StreamT& w) { w.writeVec2fLittle(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec2d, Endian::Little>(const PropId& id, atVec2d& var, Write::StreamT& w) { w.writeVec2dLittle(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec3f, Endian::Little>(const PropId& id, atVec3f& var, Write::StreamT& w) { w.writeVec3fLittle(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec3d, Endian::Little>(const PropId& id, atVec3d& var, Write::StreamT& w) { w.writeVec3dLittle(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec4f, Endian::Little>(const PropId& id, atVec4f& var, Write::StreamT& w) { w.writeVec4fLittle(var); }
template <> template <> inline void Write<PropType::None>::Do<atVec4d, Endian::Little>(const PropId& id, atVec4d& var, Write::StreamT& w) { w.writeVec4dLittle(var); }
template <> template <> inline void Write<PropType::None>::Do<Endian::Little>(const PropId& id, std::wstring& str, StreamT& w) { w.writeWStringLittle(str); }
template <> template <> inline void Write<PropType::None>::Do<Endian::Little>(const PropId& id, std::wstring& str, size_t count, StreamT& w) { w.writeWStringLittle(str, count); }

template <PropType PropOp>
struct ReadYaml
{
    using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
    using StreamT = YAMLDocReader;
    template <class T, Endian DNAE>
    static typename std::enable_if_t<std::is_enum_v<T>>
    Do(const PropId& id, T& var, StreamT& r)
    {
        using PODType = std::underlying_type_t<T>;
        ReadYaml<PropType::None>::Do<PODType, DNAE>(id, *reinterpret_cast<PODType*>(&var), r);
    }
    template <class T, Endian DNAE>
    static typename std::enable_if_t<__IsPODType<T>()>
    Do(const PropId& id, T& var, StreamT& r)
    {
        ReadYaml<PropType::None>::Do<T, DNAE>(id, var, r);
    }
    template <class T, Endian DNAE>
    static typename std::enable_if_t<!std::is_enum_v<T> && !__IsPODType<T>()>
    Do(const PropId& id, T& var, StreamT& r)
    {
        if (auto rec = r.enterSubRecord(id.name))
            var.template Enumerate<ReadYaml<PropOp>>(r);
    }
    template <class T, Endian DNAE>
    static void Do(const PropId& id, std::vector<T>& vector, size_t count, StreamT& r)
    {
        vector.clear();
        vector.reserve(count);
        for (size_t i = 0; i < count; ++i)
        {
            vector.emplace_back();
            ReadYaml<PropOp>::Do<T, DNAE>(id, vector.back(), r);
        }
    }
    static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& r)
    {
        buf = r.readUBytes(id.name);
    }
    static void Do(const PropId& id, std::string& str, StreamT& r)
    {
        str = r.readString(id.name);
    }
    static void Do(const PropId& id, std::string& str, size_t count, StreamT& r)
    {
        str = r.readString(id.name);
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, StreamT& r)
    {
        str = r.readWString(id.name);
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, size_t count, StreamT& r)
    {
        str = r.readWString(id.name);
    }
    static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& r) {}
    static void DoAlign(atInt64 amount, StreamT& r) {}
};
template <> template <> inline void ReadYaml<PropType::None>::Do<bool, Endian::Big>(const PropId& id, bool& var, ReadYaml::StreamT& r) { var = r.readBool(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atInt8, Endian::Big>(const PropId& id, atInt8& var, ReadYaml::StreamT& r) { var = r.readByte(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atUint8, Endian::Big>(const PropId& id, atUint8& var, ReadYaml::StreamT& r) { var = r.readUByte(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atInt16, Endian::Big>(const PropId& id, atInt16& var, ReadYaml::StreamT& r) { var = r.readInt16(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atUint16, Endian::Big>(const PropId& id, atUint16& var, ReadYaml::StreamT& r) { var = r.readUint16(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atInt32, Endian::Big>(const PropId& id, atInt32& var, ReadYaml::StreamT& r) { var = r.readInt32(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atUint32, Endian::Big>(const PropId& id, atUint32& var, ReadYaml::StreamT& r) { var = r.readUint32(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atInt64, Endian::Big>(const PropId& id, atInt64& var, ReadYaml::StreamT& r) { var = r.readInt64(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atUint64, Endian::Big>(const PropId& id, atUint64& var, ReadYaml::StreamT& r) { var = r.readUint64(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<float, Endian::Big>(const PropId& id, float& var, ReadYaml::StreamT& r) { var = r.readFloat(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<double, Endian::Big>(const PropId& id, double& var, ReadYaml::StreamT& r) { var = r.readDouble(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec2f, Endian::Big>(const PropId& id, atVec2f& var, ReadYaml::StreamT& r) { var = r.readVec2f(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec2d, Endian::Big>(const PropId& id, atVec2d& var, ReadYaml::StreamT& r) { var = r.readVec2d(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec3f, Endian::Big>(const PropId& id, atVec3f& var, ReadYaml::StreamT& r) { var = r.readVec3f(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec3d, Endian::Big>(const PropId& id, atVec3d& var, ReadYaml::StreamT& r) { var = r.readVec3d(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec4f, Endian::Big>(const PropId& id, atVec4f& var, ReadYaml::StreamT& r) { var = r.readVec4f(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec4d, Endian::Big>(const PropId& id, atVec4d& var, ReadYaml::StreamT& r) { var = r.readVec4d(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<bool, Endian::Little>(const PropId& id, bool& var, ReadYaml::StreamT& r) { var = r.readBool(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atInt8, Endian::Little>(const PropId& id, atInt8& var, ReadYaml::StreamT& r) { var = r.readByte(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atUint8, Endian::Little>(const PropId& id, atUint8& var, ReadYaml::StreamT& r) { var = r.readUByte(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atInt16, Endian::Little>(const PropId& id, atInt16& var, ReadYaml::StreamT& r) { var = r.readInt16(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atUint16, Endian::Little>(const PropId& id, atUint16& var, ReadYaml::StreamT& r) { var = r.readUint16(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atInt32, Endian::Little>(const PropId& id, atInt32& var, ReadYaml::StreamT& r) { var = r.readInt32(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atUint32, Endian::Little>(const PropId& id, atUint32& var, ReadYaml::StreamT& r) { var = r.readUint32(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atInt64, Endian::Little>(const PropId& id, atInt64& var, ReadYaml::StreamT& r) { var = r.readInt64(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atUint64, Endian::Little>(const PropId& id, atUint64& var, ReadYaml::StreamT& r) { var = r.readUint64(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<float, Endian::Little>(const PropId& id, float& var, ReadYaml::StreamT& r) { var = r.readFloat(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<double, Endian::Little>(const PropId& id, double& var, ReadYaml::StreamT& r) { var = r.readDouble(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec2f, Endian::Little>(const PropId& id, atVec2f& var, ReadYaml::StreamT& r) { var = r.readVec2f(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec2d, Endian::Little>(const PropId& id, atVec2d& var, ReadYaml::StreamT& r) { var = r.readVec2d(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec3f, Endian::Little>(const PropId& id, atVec3f& var, ReadYaml::StreamT& r) { var = r.readVec3f(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec3d, Endian::Little>(const PropId& id, atVec3d& var, ReadYaml::StreamT& r) { var = r.readVec3d(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec4f, Endian::Little>(const PropId& id, atVec4f& var, ReadYaml::StreamT& r) { var = r.readVec4f(id.name); }
template <> template <> inline void ReadYaml<PropType::None>::Do<atVec4d, Endian::Little>(const PropId& id, atVec4d& var, ReadYaml::StreamT& r) { var = r.readVec4d(id.name); }

template <PropType PropOp>
struct WriteYaml
{
    using PropT = std::conditional_t<PropOp == PropType::CRC64, uint64_t, uint32_t>;
    using StreamT = YAMLDocWriter;
    template <class T, Endian DNAE>
    static typename std::enable_if_t<std::is_enum_v<T>>
    Do(const PropId& id, T& var, StreamT& w)
    {
        using PODType = std::underlying_type_t<T>;
        WriteYaml<PropType::None>::Do<PODType, DNAE>(id, *reinterpret_cast<PODType*>(&var), w);
    }
    template <class T, Endian DNAE>
    static typename std::enable_if_t<__IsPODType<T>()>
    Do(const PropId& id, T& var, StreamT& w)
    {
        WriteYaml<PropType::None>::Do<T, DNAE>(id, var, w);
    }
    template <class T, Endian DNAE>
    static typename std::enable_if_t<!std::is_enum_v<T> && !__IsPODType<T>()>
    Do(const PropId& id, T& var, StreamT& w)
    {
        if (auto rec = w.enterSubRecord(id.name))
            var.template Enumerate<WriteYaml<PropOp>>(w);
    }
    template <class T, Endian DNAE>
    static void Do(const PropId& id, std::vector<T>& vector, size_t count, StreamT& w)
    {
        for (T& v : vector)
            WriteYaml<PropOp>::Do<T, DNAE>(id, v, w);
    }
    static void Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, StreamT& w)
    {
        w.writeUBytes(id.name, buf, count);
    }
    static void Do(const PropId& id, std::string& str, StreamT& w)
    {
        w.writeString(id.name, str);
    }
    static void Do(const PropId& id, std::string& str, size_t count, StreamT& w)
    {
        w.writeString(id.name, str);
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, StreamT& w)
    {
        w.writeWString(id.name, str);
    }
    template <Endian DNAE>
    static void Do(const PropId& id, std::wstring& str, size_t count, StreamT& w)
    {
        w.writeWString(id.name, str);
    }
    static void DoSeek(atInt64 amount, SeekOrigin whence, StreamT& w) {}
    static void DoAlign(atInt64 amount, StreamT& w) {}
};
template <> template <> inline void WriteYaml<PropType::None>::Do<bool, Endian::Big>(const PropId& id, bool& var, WriteYaml::StreamT& w) { w.writeBool(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atInt8, Endian::Big>(const PropId& id, atInt8& var, WriteYaml::StreamT& w) { w.writeByte(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atUint8, Endian::Big>(const PropId& id, atUint8& var, WriteYaml::StreamT& w) { w.writeUByte(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atInt16, Endian::Big>(const PropId& id, atInt16& var, WriteYaml::StreamT& w) { w.writeInt16(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atUint16, Endian::Big>(const PropId& id, atUint16& var, WriteYaml::StreamT& w) { w.writeUint16(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atInt32, Endian::Big>(const PropId& id, atInt32& var, WriteYaml::StreamT& w) { w.writeInt32(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atUint32, Endian::Big>(const PropId& id, atUint32& var, WriteYaml::StreamT& w) { w.writeUint32(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atInt64, Endian::Big>(const PropId& id, atInt64& var, WriteYaml::StreamT& w) { w.writeInt64(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atUint64, Endian::Big>(const PropId& id, atUint64& var, WriteYaml::StreamT& w) { w.writeUint64(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<float, Endian::Big>(const PropId& id, float& var, WriteYaml::StreamT& w) { w.writeFloat(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<double, Endian::Big>(const PropId& id, double& var, WriteYaml::StreamT& w) { w.writeDouble(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec2f, Endian::Big>(const PropId& id, atVec2f& var, WriteYaml::StreamT& w) { w.writeVec2f(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec2d, Endian::Big>(const PropId& id, atVec2d& var, WriteYaml::StreamT& w) { w.writeVec2d(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec3f, Endian::Big>(const PropId& id, atVec3f& var, WriteYaml::StreamT& w) { w.writeVec3f(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec3d, Endian::Big>(const PropId& id, atVec3d& var, WriteYaml::StreamT& w) { w.writeVec3d(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec4f, Endian::Big>(const PropId& id, atVec4f& var, WriteYaml::StreamT& w) { w.writeVec4f(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec4d, Endian::Big>(const PropId& id, atVec4d& var, WriteYaml::StreamT& w) { w.writeVec4d(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<bool, Endian::Little>(const PropId& id, bool& var, WriteYaml::StreamT& w) { w.writeBool(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atInt8, Endian::Little>(const PropId& id, atInt8& var, WriteYaml::StreamT& w) { w.writeByte(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atUint8, Endian::Little>(const PropId& id, atUint8& var, WriteYaml::StreamT& w) { w.writeUByte(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atInt16, Endian::Little>(const PropId& id, atInt16& var, WriteYaml::StreamT& w) { w.writeInt16(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atUint16, Endian::Little>(const PropId& id, atUint16& var, WriteYaml::StreamT& w) { w.writeUint16(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atInt32, Endian::Little>(const PropId& id, atInt32& var, WriteYaml::StreamT& w) { w.writeInt32(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atUint32, Endian::Little>(const PropId& id, atUint32& var, WriteYaml::StreamT& w) { w.writeUint32(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atInt64, Endian::Little>(const PropId& id, atInt64& var, WriteYaml::StreamT& w) { w.writeInt64(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atUint64, Endian::Little>(const PropId& id, atUint64& var, WriteYaml::StreamT& w) { w.writeUint64(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<float, Endian::Little>(const PropId& id, float& var, WriteYaml::StreamT& w) { w.writeFloat(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<double, Endian::Little>(const PropId& id, double& var, WriteYaml::StreamT& w) { w.writeDouble(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec2f, Endian::Little>(const PropId& id, atVec2f& var, WriteYaml::StreamT& w) { w.writeVec2f(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec2d, Endian::Little>(const PropId& id, atVec2d& var, WriteYaml::StreamT& w) { w.writeVec2d(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec3f, Endian::Little>(const PropId& id, atVec3f& var, WriteYaml::StreamT& w) { w.writeVec3f(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec3d, Endian::Little>(const PropId& id, atVec3d& var, WriteYaml::StreamT& w) { w.writeVec3d(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec4f, Endian::Little>(const PropId& id, atVec4f& var, WriteYaml::StreamT& w) { w.writeVec4f(id.name, var); }
template <> template <> inline void WriteYaml<PropType::None>::Do<atVec4d, Endian::Little>(const PropId& id, atVec4d& var, WriteYaml::StreamT& w) { w.writeVec4d(id.name, var); }

template <class Op, class T, Endian DNAE>
static inline void __Do(const PropId& id, T& var, typename Op::StreamT& s)
{
    Op::template Do<T, DNAE>(id, var, s);
}

template <class Op, class T, Endian DNAE>
static inline void __Do(const PropId& id, std::vector<T>& vector, size_t count, typename Op::StreamT& s)
{
    Op::template Do<T, DNAE>(id, vector, count, s);
}

template <class Op>
static inline void __Do(const PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, typename Op::StreamT& s)
{
    Op::Do(id, buf, count, s);
}

template <class Op>
static inline void __Do(const PropId& id, std::string& str, typename Op::StreamT& s)
{
    Op::Do(id, str, s);
}

template <class Op>
static inline void __Do(const PropId& id, std::string& str, size_t count, typename Op::StreamT& s)
{
    Op::Do(id, str, count, s);
}

template <class Op, Endian DNAE>
static inline void __Do(const PropId& id, std::wstring& str, typename Op::StreamT& s)
{
    Op::template Do<DNAE>(id, str, s);
}

template <class Op, Endian DNAE>
static inline void __Do(const PropId& id, std::wstring& str, size_t count, typename Op::StreamT& s)
{
    Op::template Do<DNAE>(id, str, count, s);
}

template <class Op>
static inline void __DoSeek(atInt64 delta, athena::SeekOrigin whence, typename Op::StreamT& s)
{
    Op::DoSeek(delta, whence, s);
}

template <class Op>
static inline void __DoAlign(atInt64 amount, typename Op::StreamT& s)
{
    Op::DoAlign(amount, s);
}

template <class T>
static inline void __Read(T& obj, athena::io::IStreamReader& r)
{
    __Do<Read<PropType::None>, T, T::DNAEndian>({}, obj, r);
}

template <class T>
static inline void __Write(const T& obj, athena::io::IStreamWriter& w)
{
    __Do<Write<PropType::None>, T, T::DNAEndian>({}, const_cast<T&>(obj), w);
}

template <class T>
static inline void __BinarySize(const T& obj, size_t& s)
{
    __Do<BinarySize<PropType::None>, T, T::DNAEndian>({}, const_cast<T&>(obj), s);
}

template <class T>
static inline void __PropCount(const T& obj, size_t& s)
{
    const_cast<T&>(obj).template Enumerate<PropCount<PropType::None>>(s);
}

template <class T>
static inline void __ReadYaml(T& obj, athena::io::YAMLDocReader& r)
{
    obj.template Enumerate<ReadYaml<PropType::None>>(r);
}

template <class T>
static inline void __WriteYaml(const T& obj, athena::io::YAMLDocWriter& w)
{
    const_cast<T&>(obj).template Enumerate<WriteYaml<PropType::None>>(w);
}

template <class T>
static inline void __ReadProp(T& obj, athena::io::IStreamReader& r)
{
    /* Read root 0xffffffff hash (hashed empty string) */
    atUint32 hash = T::DNAEndian == Endian::Big ? r.readUint32Big() : r.readUint32Little();
    atInt64 size = T::DNAEndian == Endian::Big ? r.readUint16Big() : r.readUint16Little();
    atInt64 start = r.position();
    __Do<Read<PropType::CRC32>, T, T::DNAEndian>({}, obj, r);
    atInt64 actualRead = r.position() - start;
    if (actualRead != size)
        r.seek(size - actualRead);
}

template <class T>
static inline void __WriteProp(const T& obj, athena::io::IStreamWriter& w)
{
    __Do<Write<PropType::CRC32>, T, T::DNAEndian>({}, const_cast<T&>(obj), w);
}

template <class T>
static inline void __BinarySizeProp(const T& obj, size_t& s)
{
    __Do<BinarySize<PropType::CRC32>, T, T::DNAEndian>({}, const_cast<T&>(obj), s);
}

template <class T>
static inline void __ReadProp64(T& obj, athena::io::IStreamReader& r)
{
    /* Read root 0x0 hash (hashed empty string) */
    atUint64 hash = T::DNAEndian == Endian::Big ? r.readUint64Big() : r.readUint64Little();
    atInt64 size = T::DNAEndian == Endian::Big ? r.readUint16Big() : r.readUint16Little();
    atInt64 start = r.position();
    __Do<Read<PropType::CRC64>, T, T::DNAEndian>({}, obj, r);
    atInt64 actualRead = r.position() - start;
    if (actualRead != size)
        r.seek(size - actualRead);
}

template <class T>
static inline void __WriteProp64(const T& obj, athena::io::IStreamWriter& w)
{
    __Do<Write<PropType::CRC64>, T, T::DNAEndian>({}, const_cast<T&>(obj), w);
}

template <class T>
static inline void __BinarySizeProp64(const T& obj, size_t& s)
{
    __Do<BinarySize<PropType::CRC64>, T, T::DNAEndian>({}, const_cast<T&>(obj), s);
}

}

#define AT_DECL_DNA \
template <class Op, athena::Endian DNAE = DNAEndian, class T> \
void Do(const athena::io::PropId& id, T& var, typename Op::StreamT& s) { athena::io::__Do<Op, T, DNAE>(id, var, s); } \
template <class Op, athena::Endian DNAE = DNAEndian, class T> \
void Do(const athena::io::PropId& id, std::vector<T>& var, size_t count, typename Op::StreamT& s) { athena::io::__Do<Op, T, DNAE>(id, var, count, s); } \
template <class Op> \
void Do(const athena::io::PropId& id, std::unique_ptr<atUint8[]>& buf, size_t count, typename Op::StreamT& s) { athena::io::__Do<Op>(id, buf, count, s); } \
template <class Op> \
void Do(const athena::io::PropId& id, std::string& str, typename Op::StreamT& s) { athena::io::__Do<Op>(id, str, s); } \
template <class Op> \
void Do(const athena::io::PropId& id, std::string& str, size_t count, typename Op::StreamT& s) { athena::io::__Do<Op>(id, str, count, s); } \
template <class Op, athena::Endian DNAE = DNAEndian> \
void Do(const athena::io::PropId& id, std::wstring& str, typename Op::StreamT& s) { athena::io::__Do<Op, DNAE>(id, str, s); } \
template <class Op, athena::Endian DNAE = DNAEndian> \
void Do(const athena::io::PropId& id, std::wstring& str, size_t count, typename Op::StreamT& s) { athena::io::__Do<Op, DNAE>(id, str, count, s); } \
template <class Op> \
void DoSeek(atInt64 delta, athena::SeekOrigin whence, typename Op::StreamT& s) { athena::io::__DoSeek<Op>(delta, whence, s); } \
template <class Op> \
void DoAlign(atInt64 amount, typename Op::StreamT& s) { athena::io::__DoAlign<Op>(amount, s); } \
template <class Op> \
void Enumerate(typename Op::StreamT& s); \
template <class Op> \
bool Lookup(uint64_t hash, typename Op::StreamT& s); \
void read(athena::io::IStreamReader& r) { athena::io::__Read(*this, r); } \
void write(athena::io::IStreamWriter& w) const { athena::io::__Write(*this, w); } \
void binarySize(size_t& s) const { athena::io::__BinarySize(*this, s); } \
void read(athena::io::YAMLDocReader& r) { athena::io::__ReadYaml(*this, r); } \
void write(athena::io::YAMLDocWriter& w) const { athena::io::__WriteYaml(*this, w); } \
void readProp(athena::io::IStreamReader& r) { athena::io::__ReadProp(*this, r); } \
void writeProp(athena::io::IStreamWriter& w) const { athena::io::__WriteProp(*this, w); } \
void binarySizeProp(size_t& s) const { athena::io::__BinarySizeProp(*this, s); } \
void propCount(size_t& s) const { athena::io::__PropCount(*this, s); } \
void readProp64(athena::io::IStreamReader& r) { athena::io::__ReadProp64(*this, r); } \
void writeProp64(athena::io::IStreamWriter& w) const { athena::io::__WriteProp64(*this, w); } \
void binarySizeProp64(size_t& s) const { athena::io::__BinarySizeProp64(*this, s); } \

#define AT_SPECIALIZE_DNA(__Type) \
template void __Type::Enumerate<athena::io::Read<athena::io::PropType::None>>(athena::io::Read<athena::io::PropType::None>::StreamT& s); \
template void __Type::Enumerate<athena::io::Write<athena::io::PropType::None>>(athena::io::Write<athena::io::PropType::None>::StreamT& s); \
template void __Type::Enumerate<athena::io::BinarySize<athena::io::PropType::None>>(athena::io::BinarySize<athena::io::PropType::None>::StreamT& s); \
template void __Type::Enumerate<athena::io::ReadYaml<athena::io::PropType::None>>(athena::io::ReadYaml<athena::io::PropType::None>::StreamT& s); \
template void __Type::Enumerate<athena::io::WriteYaml<athena::io::PropType::None>>(athena::io::WriteYaml<athena::io::PropType::None>::StreamT& s); \
template bool __Type::Lookup<athena::io::Read<athena::io::PropType::CRC32>>(uint64_t hash, athena::io::Read<athena::io::PropType::CRC32>::StreamT& s); \
template void __Type::Enumerate<athena::io::Read<athena::io::PropType::CRC32>>(athena::io::Read<athena::io::PropType::CRC32>::StreamT& s); \
template bool __Type::Lookup<athena::io::Write<athena::io::PropType::CRC32>>(uint64_t hash, athena::io::Write<athena::io::PropType::CRC32>::StreamT& s); \
template void __Type::Enumerate<athena::io::Write<athena::io::PropType::CRC32>>(athena::io::Write<athena::io::PropType::CRC32>::StreamT& s); \
template bool __Type::Lookup<athena::io::BinarySize<athena::io::PropType::CRC32>>(uint64_t hash, athena::io::BinarySize<athena::io::PropType::CRC32>::StreamT& s); \
template void __Type::Enumerate<athena::io::BinarySize<athena::io::PropType::CRC32>>(athena::io::BinarySize<athena::io::PropType::CRC32>::StreamT& s); \
template bool __Type::Lookup<athena::io::PropCount<athena::io::PropType::CRC32>>(uint64_t hash, athena::io::PropCount<athena::io::PropType::CRC32>::StreamT& s); \
template void __Type::Enumerate<athena::io::PropCount<athena::io::PropType::CRC32>>(athena::io::PropCount<athena::io::PropType::CRC32>::StreamT& s); \
template bool __Type::Lookup<athena::io::ReadYaml<athena::io::PropType::CRC32>>(uint64_t hash, athena::io::ReadYaml<athena::io::PropType::CRC32>::StreamT& s); \
template void __Type::Enumerate<athena::io::ReadYaml<athena::io::PropType::CRC32>>(athena::io::ReadYaml<athena::io::PropType::CRC32>::StreamT& s); \
template bool __Type::Lookup<athena::io::WriteYaml<athena::io::PropType::CRC32>>(uint64_t hash, athena::io::WriteYaml<athena::io::PropType::CRC32>::StreamT& s); \
template void __Type::Enumerate<athena::io::WriteYaml<athena::io::PropType::CRC32>>(athena::io::WriteYaml<athena::io::PropType::CRC32>::StreamT& s); \
template bool __Type::Lookup<athena::io::Read<athena::io::PropType::CRC64>>(uint64_t hash, athena::io::Read<athena::io::PropType::CRC64>::StreamT& s); \
template void __Type::Enumerate<athena::io::Read<athena::io::PropType::CRC64>>(athena::io::Read<athena::io::PropType::CRC64>::StreamT& s); \
template bool __Type::Lookup<athena::io::Write<athena::io::PropType::CRC64>>(uint64_t hash, athena::io::Write<athena::io::PropType::CRC64>::StreamT& s); \
template void __Type::Enumerate<athena::io::Write<athena::io::PropType::CRC64>>(athena::io::Write<athena::io::PropType::CRC64>::StreamT& s); \
template bool __Type::Lookup<athena::io::BinarySize<athena::io::PropType::CRC64>>(uint64_t hash, athena::io::BinarySize<athena::io::PropType::CRC64>::StreamT& s); \
template void __Type::Enumerate<athena::io::BinarySize<athena::io::PropType::CRC64>>(athena::io::BinarySize<athena::io::PropType::CRC64>::StreamT& s); \
template bool __Type::Lookup<athena::io::PropCount<athena::io::PropType::CRC64>>(uint64_t hash, athena::io::PropCount<athena::io::PropType::CRC64>::StreamT& s); \
template void __Type::Enumerate<athena::io::PropCount<athena::io::PropType::CRC64>>(athena::io::PropCount<athena::io::PropType::CRC64>::StreamT& s); \
template bool __Type::Lookup<athena::io::ReadYaml<athena::io::PropType::CRC64>>(uint64_t hash, athena::io::ReadYaml<athena::io::PropType::CRC64>::StreamT& s); \
template void __Type::Enumerate<athena::io::ReadYaml<athena::io::PropType::CRC64>>(athena::io::ReadYaml<athena::io::PropType::CRC64>::StreamT& s); \
template bool __Type::Lookup<athena::io::WriteYaml<athena::io::PropType::CRC64>>(uint64_t hash, athena::io::WriteYaml<athena::io::PropType::CRC64>::StreamT& s); \
template void __Type::Enumerate<athena::io::WriteYaml<athena::io::PropType::CRC64>>(athena::io::WriteYaml<athena::io::PropType::CRC64>::StreamT& s);

using namespace athena::io::literals;

#endif // AT_OP_HPP
