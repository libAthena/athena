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
    using Buffer = std::unique_ptr<atUint8[]>;

    template <atInt32 sizeVar = -1>
    using String = std::string;

    template <atInt32 sizeVar = -1, Endian VE = DNAE>
    using WString = std::wstring;

    template <atInt32 sizeVar = -1>
    using UTF8 = std::string;

    template <off_t offset, SeekOrigin direction>
    struct Seek {};

    template <size_t align>
    struct Align {};

    virtual void read(IStreamReader&)=0;
    virtual void write(IStreamWriter&) const=0;
};

/** Macro to automatically declare read/write methods in subclasses */
#define DECL_DNA \
    void read(Athena::io::IStreamReader&); \
    void write(Athena::io::IStreamWriter&) const; \

/** Macro to supply count variable to atdna and mute it for other compilers */
#ifdef __clang__
#define DNA_COUNT(cnt) sizeof(cnt)
#else
#define DNA_COUNT(cnt) 0
#endif

}
}

#endif // DNA_HPP

