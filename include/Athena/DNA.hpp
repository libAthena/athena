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

    virtual void read(IStreamReader&)=0;
    virtual void write(IStreamWriter&) const=0;
};

/** Macro to automatically declare read/write methods in subclasses */
#define DECL_DNA \
    void read(Athena::io::IStreamReader&); \
    void write(Athena::io::IStreamWriter&) const; \

}
}

#endif // DNA_HPP

