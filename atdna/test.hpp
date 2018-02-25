#include <athena/DNAYaml.hpp>

using namespace athena;
typedef io::DNA<Big> BigDNA;

enum ETest : atUint8
{
    ZERO,
    ONE,
    TWO,
    THREE
};

template <ETest EVal>
struct AT_SPECIALIZE_PARMS(ETest::ZERO, ETest::ONE, ETest::TWO, ETest::THREE)
TESTSubFile : public BigDNA
{
    AT_DECL_DNA
    Value<ETest> varE = EVal;
    Value<atUint32> sub1;
    Value<atUint32> sub2;
};

struct TESTSubClassFile : public TESTSubFile<ETest::ONE>
{
    AT_DECL_DNA
    Value<atUint32> sub3;
    Value<atUint16> sub4;
};

struct TESTSubSubClassFile : public TESTSubClassFile
{
    AT_DECL_DNA
    Value<atUint32> sub5;
    Value<atUint32> sub6;
};

template <class Var32Tp, int Var32Val>
struct AT_SPECIALIZE_PARMS(atUint16, 42, atUint32, 87, atUint32, 2)
TESTFile : public BigDNA
{
    AT_DECL_DNA
    Value<bool> varBool;
    AT_OVERRIDE_RCRC32(12345678) Value<Var32Tp> x4_var32 = Var32Val;
    AT_OVERRIDE_RCRC32(deadbabe) Value<atUint16> x8_var16;
    Value<atVec3f> vec3;
    Value<atVec4f> vec4;

    struct TESTNestedSubFile : public BigDNA
    {
        AT_DECL_DNA
        Value<atUint32> nestSub1;
        Value<atUint32> nestSub2;
    } nestedSubFile; 

    using TESTSubFileUsing = TESTSubFile<ETest::TWO>;
    TESTSubFileUsing subFile;

    Align<4> align;

    template <class NestedTp, int NestedVal>
    struct AT_SPECIALIZE_PARMS(atInt32, 36, atInt64, 96)
    TESTTemplateSubFile : public BigDNA
    {
        AT_DECL_DNA
        Value<NestedTp> explSub1 = NestedVal;
        Value<Var32Tp> explSub2 = Var32Val;
    };
    TESTTemplateSubFile<atInt32, 36> nestedTemplate1;
    TESTTemplateSubFile<atInt64, 96> nestedTemplate2;

    Value<atUint32, Little> arrCount[2];
    Vector<atUint32, AT_DNA_COUNT(arrCount[0])> array;

    Value<atUint32> arrAltCount;
    Vector<atUint32, AT_DNA_COUNT(arrAltCount)> arrayAlt;

    Seek<21, Current> seek;

    Value<atUint32> arrCount2;
    Vector<TESTSubFile<ETest::ZERO>, AT_DNA_COUNT(arrCount[1] + arrCount2)> array2;

    Value<atUint32> bufSz;
    Buffer<AT_DNA_COUNT(bufSz)> buf;

    String<32> str;
    WString<64> wstr;
};

