#include <stdint.h>
#include <vector>
typedef uint32_t atUint32;
typedef uint16_t atUint16;

enum Endian
{
    LITTLE,
    BIG
};

template <typename T, Endian endian>
using Value = T;

template <typename T, Endian endian>
using Vector = std::vector<T>;

namespace Athena
{
namespace io
{
class IStreamReader;
class IStreamWriter;
struct DNA
{
    void read(IStreamReader&);
    void write(IStreamWriter&) const;
};
}
}

using namespace Athena;

struct ANCSFile : public io::DNA
{
    Value<atUint32, BIG> var32;
    Value<atUint16, BIG> var16;
    Vector<atUint32, BIG> vec;
};
