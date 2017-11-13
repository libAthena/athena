#ifndef DNA_HPP
#define DNA_HPP

/* BIG FAT WARNING!!!
 *
 * The type-structure of this file is expected to remain consistent for 'atdna'
 * Any changes to the types or namespacing must be reflected in 'atdna/main.cpp'
 */

#include "Global.hpp"
#include "IStreamReader.hpp"
#include "IStreamWriter.hpp"
#include <vector>
#include <memory>

namespace athena
{
namespace io
{

/* forward-declaration dance for recursively-derived types */

template <size_t sizeVar, Endian VE>
struct Buffer;

template <atInt32 sizeVar, Endian VE>
struct String;

template <atInt32 sizeVar, Endian VE>
struct WString;

template <atInt32 sizeVar, Endian VE>
struct WStringAsString;

/**
 * @brief Base DNA class used against 'atdna'
 * @tparam DNAE Default-endianness for contained DNA values
 *
 * Athena bundles a build-tool called 'atdna'. This tool functions
 * just like the 'clang' compiler, except it emits a full .cpp implementation
 * with all read/write calls necessary to marshal the DNA structure to/from
 * a streamed medium
 */
template <Endian DNAE>
struct DNA
{
    virtual ~DNA() = default;

    /**
     * @brief Common virtual read function for all DNA types
     */
    virtual void read(IStreamReader&)=0;
    /**
     * @brief Common virtual write function for all DNA types
     */
    virtual void write(IStreamWriter&) const=0;
    /**
     * @brief Common virtual binary size computation for all DNA types
     * @param __isz initial cumulative value to add result to
     * @return Cumulative size
     */
    virtual size_t binarySize(size_t __isz) const=0;

    /**
     * @brief Template type signaling atdna to capture the value where it's used
     * @tparam T The type of the value. Can be any numeric type or atVec* type
     * @tparam VE Endianness of the value
     */
    template <typename T, Endian VE = DNAE>
    using Value = T;

    /**
     * @brief Template type wrapping std::vector and signaling atdna to manipulate it where it's used
     * @tparam T The type of contained elements. Can be any numeric type, atVec* type, or another DNA subclass
     * @tparam cntVar C++ expression wrapped in DNA_COUNT macro to determine number of elements for vector
     * @tparam VE Endianness of the contained values
     */
    template <typename T, size_t cntVar, Endian VE = DNAE>
    using Vector = std::vector<T>;

    /**
     * @brief Template type wrapping std::unique_ptr<atUint8[]> and signaling atdna to read a
     *        raw byte-buffer where it's used
     * @tparam sizeVar C++ expression wrapped in DNA_COUNT macro to determine number of bytes for buffer
     */
    template <size_t sizeVar>
    using Buffer = struct athena::io::Buffer<sizeVar, DNAE>;

    /**
     * @brief Template type wrapping std::string and signaling atdna to read string data where it's used
     * @tparam sizeVar C++ expression wrapped in DNA_COUNT macro to determine number of characters for string
     *                 -1 literal indicates null-terminated string
     */
    template <atInt32 sizeVar = -1>
    using String = struct athena::io::String<sizeVar, DNAE>;

    /**
     * @brief Template type wrapping std::wstring and signaling atdna to read wstring data where it's used
     * @tparam sizeVar C++ expression wrapped in DNA_COUNT macro to determine number of characters for wstring
     *                 -1 literal indicates null-terminated wstring
     */
    template <atInt32 sizeVar = -1, Endian VE = DNAE>
    using WString = struct athena::io::WString<sizeVar, VE>;

    /**
     * @brief Template type wrapping std::string and signaling atdna to read wstring data where it's used
     * @tparam sizeVar C++ expression wrapped in DNA_COUNT macro to determine number of characters for string
     *                 -1 literal indicates null-terminated string
     */
    template <atInt32 sizeVar = -1>
    using WStringAsString = struct athena::io::WStringAsString<sizeVar, DNAE>;

    /**
     * @brief Meta Template signaling atdna to insert a stream seek where it's used
     * @tparam offset C++ expression wrapped in DNA_COUNT macro to determine number of bytes to seek
     * @tparam direction SeekOrigin to seek relative to
     */
    template <off_t offset, SeekOrigin direction>
    struct Seek {};

    /**
     * @brief Meta Template signaling atdna to insert an aligning stream seek where it's used
     * @tparam align Number of bytes to align to
     */
    template <size_t align>
    struct Align {};

    /**
     * @brief Meta Template preventing atdna from emitting read/write implementations
     */
    struct Delete {};

    /**
     * @brief Internal DNA helper for accumulating binarySize
     * @param __isz initial size value
     * @param v Vector to enumerate
     * @return Cumulative total
     */
    template <typename T>
    static size_t __EnumerateSize(size_t __isz, const T& v)
    {
        for (const auto& val : v)
            __isz = val.binarySize(__isz);
        return __isz;
    }
};

/**
 * @brief Concrete buffer type used by DNA::Buffer
 */
template <size_t sizeVar, Endian VE>
struct Buffer : public DNA<VE>, public std::unique_ptr<atUint8[]>
{
    typename DNA<VE>::Delete expl;
    void read(IStreamReader& reader)
    {
        reset(new atUint8[sizeVar]);
        reader.readUBytesToBuf(get(), sizeVar);
    }
    void write(IStreamWriter& writer) const
    {
        writer.writeUBytes(get(), sizeVar);
    }
    size_t binarySize(size_t __isz) const
    {
        return __isz + sizeVar;
    }
};

/**
 * @brief Concrete string type used by DNA::String
 */
template <atInt32 sizeVar, Endian VE>
struct String : public DNA<VE>, public std::string
{
    typename DNA<VE>::Delete expl;
    void read(IStreamReader& reader)
    {this->assign(std::move(reader.readString(sizeVar)));}
    void write(IStreamWriter& writer) const
    {writer.writeString(*this, sizeVar);}
    size_t binarySize(size_t __isz) const
    {return __isz + ((sizeVar<0)?(this->size()+1):sizeVar);}
    std::string& operator=(std::string_view __str)
    {return this->assign(__str);}
    std::string& operator=(std::string&& __str)
    {this->swap(__str); return *this;}
};

/**
 * @brief Concrete wstring type used by DNA::WString
 */
template <atInt32 sizeVar, Endian VE>
struct WString : public DNA<VE>, public std::wstring
{
    typename DNA<VE>::Delete expl;
    void read(IStreamReader& reader)
    {
        reader.setEndian(VE);
        this->assign(std::move(reader.readWString(sizeVar)));
    }
    void write(IStreamWriter& writer) const
    {
        writer.setEndian(VE);
        writer.writeWString(*this, sizeVar);
    }
    size_t binarySize(size_t __isz) const
    {return __isz + (((sizeVar<0)?(this->size()+1):sizeVar)*2);}
    std::wstring& operator=(std::wstring_view __str)
    {return this->assign(__str);}
    std::wstring& operator=(std::wstring&& __str)
    {this->swap(__str); return *this;}
};

/**
 * @brief Concrete converting-wstring type used by DNA::WStringAsString
 */
template <atInt32 sizeVar, Endian VE>
struct WStringAsString : public DNA<VE>, public std::string
{
    typename DNA<VE>::Delete expl;
    void read(IStreamReader& reader)
    {*this = reader.readWStringAsString(sizeVar);}
    void write(IStreamWriter& writer) const
    {writer.writeStringAsWString(*this, sizeVar);}
    size_t binarySize(size_t __isz) const
    {return __isz + (((sizeVar<0)?(this->size()+1):sizeVar)*2);}
    std::string& operator=(std::string_view __str)
    {return this->assign(__str);}
    std::string& operator=(std::string&& __str)
    {this->swap(__str); return *this;}
};

/** Macro to automatically declare read/write methods in subclasses */
#define DECL_DNA \
    void read(athena::io::IStreamReader&); \
    void write(athena::io::IStreamWriter&) const; \
    size_t binarySize(size_t __isz) const;

/** Macro to automatically declare read/write methods and prevent outputting implementation */
#define DECL_EXPLICIT_DNA \
    void read(athena::io::IStreamReader&); \
    void write(athena::io::IStreamWriter&) const; \
    size_t binarySize(size_t __isz) const; \
    Delete __dna_delete;

/** Macro to supply count variable to atdna and mute it for other compilers */
#ifdef __clang__
#define DNA_COUNT(cnt) sizeof(cnt)
#else
#define DNA_COUNT(cnt) 0
#endif

}
}

#endif // DNA_HPP

