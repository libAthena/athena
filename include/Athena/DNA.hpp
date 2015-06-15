#ifndef DNA_HPP
#define DNA_HPP

#include <vector>

namespace Athena
{
namespace io
{
class IStreamReader;
class IStreamWriter;

template <bool bigEndian=true>
struct DNA
{
    virtual bool read(IStreamReader& reader)=0;
    virtual bool write(IStreamWriter& writer) const=0;
};

template <typename T, bool bigEndian=true>
using DNAValue = T;

template <typename T, bool bigEndian=true>
using DNAVector = std::vector<T>;

}
}

#endif // DNA_HPP

