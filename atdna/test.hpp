#include <Athena/DNA.hpp>

using namespace Athena;
typedef io::DNA<BigEndian> BigDNA;

struct TESTSubFile : public BigDNA
{
    DECL_DNA
    Value<atUint32> sub1;
    Value<atUint32> sub2;
};

struct TESTSubClassFile : public TESTSubFile
{
    DECL_DNA
    Value<atUint32> sub3;
    Value<atUint32> sub4;
};

struct TESTSubSubClassFile : public TESTSubClassFile
{
    DECL_DNA
    Value<atUint32> sub5;
    Value<atUint32> sub6;
};

struct TESTFile : public BigDNA
{
    DECL_DNA
    Value<bool> varBool;
    Value<atUint32> var32;
    Value<atUint16> var16;
    Value<atVec3f> vec3;
    Value<atVec4f> vec4;

    struct TESTNestedSubFile : public BigDNA
    {
        DECL_DNA
        Value<atUint32> nestSub1;
        Value<atUint32> nestSub2;
    } nestedSubFile; 

    TESTSubFile subFile;

    Align<4> align;

    struct TESTExplicitSubFile : public BigDNA
    {
        DECL_EXPLICIT_DNA
        Value<atUint32> explSub1;
        Value<atUint32> explSub2;
    } explSubFile;

    Value<atUint32> arrCount[2];
    Vector<atUint32, DNA_COUNT(arrCount[0])> array;

    Seek<21, Current> seek;

    Value<atUint32> arrCount2;
    Vector<TESTSubFile, DNA_COUNT(arrCount[1] + arrCount2)> array2;

    Value<atUint32> bufSz;
    Buffer<DNA_COUNT(bufSz)> buf;

    String<32> str;
    WString<64> wstr;
    UTF8<> utf8str[5];
};

