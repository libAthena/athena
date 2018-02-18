#include <athena/DNAYaml.hpp>

using namespace athena;
typedef io::DNA<Big> BigDNA;

struct TESTSubFile : public BigDNA
{
    AT_DECL_DNA
    enum ETest : atUint8
    {
        ZERO,
        ONE,
        TWO,
        THREE
    };
    Value<ETest> varE;
    Value<atUint32> sub1;
    Value<atUint32> sub2;
};

struct TESTSubClassFile : public TESTSubFile
{
    AT_DECL_DNA
    Value<atUint32> sub3;
    Value<atUint32> sub4;
};

struct TESTSubSubClassFile : public TESTSubClassFile
{
    AT_DECL_DNA
    Value<atUint32> sub5;
    Value<atUint32> sub6;
};

struct TESTFile : public BigDNA
{
    AT_DECL_DNA
    Value<bool> varBool;
    AT_OVERRIDE_RCRC32(12345678) Value<atUint32> x4_var32;
    AT_OVERRIDE_RCRC32(deadbabe) Value<atUint16> x8_var16;
    Value<atVec3f> vec3;
    Value<atVec4f> vec4;

    struct TESTNestedSubFile : public BigDNA
    {
        AT_DECL_DNA
        Value<atUint32> nestSub1;
        Value<atUint32> nestSub2;
    } nestedSubFile; 

    using TESTSubFileUsing = TESTSubFile;
    TESTSubFileUsing subFile;

    Align<4> align;

    struct TESTExplicitSubFile : public BigDNA
    {
        AT_DECL_DNA
        Value<atUint32> explSub1;
        Value<atUint32> explSub2;
    } explSubFile;

    Value<atUint32, Little> arrCount[2];
    Vector<atUint32, DNA_COUNT(arrCount[0])> array;

    Seek<21, Current> seek;

    Value<atUint32> arrCount2;
    Vector<TESTSubFile, DNA_COUNT(arrCount[1] + arrCount2)> array2;

    Value<atUint32> bufSz;
    Buffer<DNA_COUNT(bufSz)> buf;

    String<32> str;
    WString<64> wstr;
};

