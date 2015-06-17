#include <Athena/DNA.hpp>

using namespace Athena;

struct ANCSSubFile : public io::DNA<BigEndian>
{
    DECL_DNA
    Value<atUint32> sub1;
    Value<atUint32> sub2;
};

struct ANCSFile : public io::DNA<BigEndian>
{
    DECL_DNA
    Value<atUint32> var32;
    Value<atUint16> var16;
    Value<atVec3f> vec3;
    Value<atVec4f> vec4;

    struct ANCSNestedSubFile : public io::DNA<BigEndian>
    {
        DECL_DNA
        Value<atUint32> nestSub1;
        Value<atUint32> nestSub2;
    } nestedSubFile;

    ANCSSubFile subFile;

    Value<atUint32> arrCount;
    Vector<atUint32, sizeof(arrCount)> array;
};

