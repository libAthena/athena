#include <stdint.h>
#include <vector>
#include <Athena/DNA.hpp>


using namespace Athena;

struct ANCSFile : public io::DNA<BigEndian>
{
    Value<atUint32> var32;
    Value<atUint16> var16;
    Vector<atUint32> vec;
};

