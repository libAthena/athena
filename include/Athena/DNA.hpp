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

/**
 * @brief Base DNA class used against 'atdna'
 *
 * Athena bundles a build-tool called 'atdna'. This tool functions
 * just like the 'clang' compiler, except it emits a full .cpp implementation
 * with all read/write calls necessary to marshal the DNA structure to/from
 * a streamed medium
 */
template <Endian DNAE>
struct DNA
{
    template <typename T, Endian VE = DNAE>
    using Value = T;

    template <typename T, size_t cntVar, Endian VE = DNAE>
    using Vector = std::vector<T>;

    template <size_t sizeVar>
    struct Buffer : public DNA, public std::unique_ptr<atUint8[]>
    {
        Delete expl;
        inline void read(IStreamReader& reader)
        {
            reset(new atUint8[sizeVar]);
            reader.readUBytesToBuf(get(), sizeVar);
        }
        inline void write(IStreamWriter& writer) const
        {
            writer.writeUBytes(get(), sizeVar);
        }
    };

    template <atInt32 sizeVar = -1>
    struct String : public DNA, public std::string
    {
        Delete expl;
        inline void read(IStreamReader& reader)
        {*this = reader.readString(sizeVar);}
        inline void write(IStreamWriter& writer) const
        {writer.writeString(*this, sizeVar);}
        inline std::string& operator=(const std::string& __str)
        {return this->assign(__str);}
        inline std::string& operator=(std::string&& __str)
        {this->swap(__str); return *this;}
    };

    template <atInt32 sizeVar = -1, Endian VE = DNAE>
    struct WString : public DNA, public std::wstring
    {
        Delete expl;
        inline void read(IStreamReader& reader)
        {
            reader.setEndian(VE);
            *this = reader.readWString(sizeVar);
        }
        inline void write(IStreamWriter& writer) const
        {
            writer.setEndian(VE);
            writer.writeWString(*this, sizeVar);
        }
        inline std::wstring& operator=(const std::wstring& __str)
        {return this->assign(__str);}
        inline std::wstring& operator=(std::wstring&& __str)
        {this->swap(__str); return *this;}
    };

    template <atInt32 sizeVar = -1>
    struct UTF8 : public DNA, public std::string
    {
        Delete expl;
        inline void read(IStreamReader& reader)
        {*this = reader.readUTF8(sizeVar);}
        inline void write(IStreamWriter& writer) const
        {writer.writeUTF8(*this, sizeVar);}
        inline std::string& operator=(const std::string& __str)
        {return this->assign(__str);}
        inline std::string& operator=(std::string&& __str)
        {this->swap(__str); return *this;}
    };

    template <off_t offset, SeekOrigin direction>
    struct Seek {};

    template <size_t align>
    struct Align {};

    struct Delete {};

    virtual void read(IStreamReader&)=0;
    virtual void write(IStreamWriter&) const=0;
};

/** Macro to automatically declare read/write methods in subclasses */
#define DECL_DNA \
    void read(Athena::io::IStreamReader&); \
    void write(Athena::io::IStreamWriter&) const; \

/** Macro to automatically declare read/write methods and prevent outputting implementation */
#define DECL_EXPLICIT_DNA \
    void read(Athena::io::IStreamReader&); \
    void write(Athena::io::IStreamWriter&) const; \
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

