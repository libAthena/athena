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

namespace Athena
{
namespace io
{

/* forward-declaration dance for recursively-derived types */

template <size_t sizeVar, Endian VE, typename STLTRAITS>
struct Buffer;

template <atInt32 sizeVar, Endian VE, typename STLTRAITS>
struct String;

template <atInt32 sizeVar, Endian VE, typename STLTRAITS>
struct WString;

template <atInt32 sizeVar, Endian VE, typename STLTRAITS>
struct WStringAsString;

/**
 * @brief Base DNA class used against 'atdna'
 *
 * Athena bundles a build-tool called 'atdna'. This tool functions
 * just like the 'clang' compiler, except it emits a full .cpp implementation
 * with all read/write calls necessary to marshal the DNA structure to/from
 * a streamed medium
 */
template <Endian DNAE, typename STLTRAITS = StlTraits>
struct DNA
{
    using IStreamReader = IStreamReader<STLTRAITS>;
    using IStreamWriter = IStreamWriter<STLTRAITS>;
    using StlTraits = STLTRAITS;

    using StlString = typename STLTRAITS::String;
    using StlWString = typename STLTRAITS::WString;
    template<typename T> using StlVector = typename STLTRAITS::template Vector<T>;

    virtual void read(IStreamReader&)=0;
    virtual void write(IStreamWriter&) const=0;

    template <typename T, Endian VE = DNAE>
    using Value = T;

    template <typename T, size_t cntVar, Endian VE = DNAE>
    using Vector = typename STLTRAITS::template Vector<T>;

    template <size_t sizeVar>
    using Buffer = struct Athena::io::Buffer<sizeVar, DNAE, STLTRAITS>;

    template <atInt32 sizeVar = -1>
    using String = struct Athena::io::String<sizeVar, DNAE, STLTRAITS>;

    template <atInt32 sizeVar = -1, Endian VE = DNAE>
    using WString = struct Athena::io::WString<sizeVar, VE, STLTRAITS>;

    template <atInt32 sizeVar = -1>
    using WStringAsString = struct Athena::io::WStringAsString<sizeVar, DNAE, STLTRAITS>;

    template <off_t offset, SeekOrigin direction>
    struct Seek {};

    template <size_t align>
    struct Align {};

    struct Delete {};
};

template <size_t sizeVar, Endian VE, typename STLTRAITS>
struct Buffer : public DNA<VE, STLTRAITS>, public std::unique_ptr<atUint8[]>
{
    typename DNA<VE>::Delete expl;
    inline void read(typename DNA<VE, STLTRAITS>::IStreamReader& reader)
    {
        reset(new atUint8[sizeVar]);
        reader.readUBytesToBuf(get(), sizeVar);
    }
    inline void write(typename DNA<VE, STLTRAITS>::IStreamWriter& writer) const
    {
        writer.writeUBytes(get(), sizeVar);
    }
};

template <atInt32 sizeVar, Endian VE, typename STLTRAITS>
struct String : public DNA<VE, STLTRAITS>, public STLTRAITS::String
{
    typename DNA<VE>::Delete expl;
    inline void read(typename DNA<VE, STLTRAITS>::IStreamReader& reader)
    {this->assign(std::move(reader.readString(sizeVar)));}
    inline void write(typename DNA<VE, STLTRAITS>::IStreamWriter& writer) const
    {writer.writeString(*this, sizeVar);}
    inline typename STLTRAITS::String& operator=(const typename STLTRAITS::String& __str)
    {return this->assign(__str);}
    inline typename STLTRAITS::String& operator=(typename STLTRAITS::String&& __str)
    {this->swap(__str); return *this;}
};

template <atInt32 sizeVar, Endian VE, typename STLTRAITS>
struct WString : public DNA<VE, STLTRAITS>, public STLTRAITS::WString
{
    typename DNA<VE>::Delete expl;
    inline void read(typename DNA<VE, STLTRAITS>::IStreamReader& reader)
    {
        reader.setEndian(VE);
        this->assign(std::move(reader.readWString(sizeVar)));
    }
    inline void write(typename DNA<VE, STLTRAITS>::IStreamWriter& writer) const
    {
        writer.setEndian(VE);
        writer.writeWString(*this, sizeVar);
    }
    inline typename STLTRAITS::WString& operator=(const typename STLTRAITS::WString& __str)
    {return this->assign(__str);}
    inline typename STLTRAITS::WString& operator=(typename STLTRAITS::WString&& __str)
    {this->swap(__str); return *this;}
};

template <atInt32 sizeVar, Endian VE, typename STLTRAITS>
struct WStringAsString : public DNA<VE, STLTRAITS>, public STLTRAITS::String
{
    typename DNA<VE>::Delete expl;
    inline void read(typename DNA<VE, STLTRAITS>::IStreamReader& reader)
    {*this = reader.readWStringAsString(sizeVar);}
    inline void write(typename DNA<VE, STLTRAITS>::IStreamWriter& writer) const
    {writer.writeStringAsWString(*this, sizeVar);}
    inline typename STLTRAITS::String& operator=(const typename STLTRAITS::String& __str)
    {return this->assign(__str);}
    inline typename STLTRAITS::String& operator=(typename STLTRAITS::String&& __str)
    {this->swap(__str); return *this;}
};

/** Macro to automatically declare read/write methods in subclasses */
#define DECL_DNA \
    void read(IStreamReader&); \
    void write(IStreamWriter&) const; \

/** Macro to automatically declare read/write methods and prevent outputting implementation */
#define DECL_EXPLICIT_DNA \
    void read(IStreamReader&); \
    void write(IStreamWriter&) const; \
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

