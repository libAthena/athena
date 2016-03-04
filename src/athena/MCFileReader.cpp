#include "athena/MCFileReader.hpp"
#include "athena/MCFile.hpp"
namespace athena
{

namespace io
{

static const atUint32 SCRAMBLE_VALUE = 0x5A424741;
MCFileReader::MCFileReader(atUint8* data, atUint64 length)
    : base(data, length)
{
}

MCFileReader::MCFileReader(const std::string& filename)
    : base(filename)
{
}

MCFile* MCFileReader::readFile()
{
    bool isScrambled = base::readUint32() != SCRAMBLE_VALUE;
    base::m_position = 0;

    if (isScrambled)
        MCFile::unscramble(base::m_dataCopy.get(), base::m_length);

    return nullptr;
}


} // io
} // zelda
