#ifndef DNA_HPP
#define DNA_HPP

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
 * just like the 'clang' compiler, except it emits
 */
template <Endian DNAE = InheritEndian>
struct DNA
{
    template <typename T, Endian VE = DNAE>
    using Value = T;

    template <typename T, Endian VE = DNAE>
    using Vector = std::vector<T>;

    virtual void read(IStreamReader&)=0;
    virtual void write(IStreamWriter&) const=0;
};

}
}

#endif // DNA_HPP

