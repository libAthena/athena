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
    Value<bool> varBool;
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

    Align<4> align;

    Value<atUint32> arrCount[2];
    Vector<atUint32, sizeof(arrCount[0])> array;

    Seek<21, Current> seek;

    Value<atUint32> arrCount2;
    Vector<ANCSSubFile, sizeof(arrCount[1] + arrCount2)> array2;

    Value<atUint32> bufSz;
    Buffer<sizeof(bufSz)> buf;

    String<32> str;
    WString<64> wstr;
    UTF8<> utf8str[5];
};

