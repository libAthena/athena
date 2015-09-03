#ifndef SSFILEREADER_HPP
#define SSFILEREADER_HPP

#include "Athena/MemoryReader.hpp"

namespace Athena
{
class SkywardSwordFile;
namespace io
{
class SkywardSwordFileReader : public MemoryCopyReader
{
    MEMORYCOPYREADER_BASE();
public:

    SkywardSwordFileReader(atUint8* data, atUint64 length);
    SkywardSwordFileReader(const std::string& filename);

    SkywardSwordFile* read();
};
} // io
} // zelda

#endif // SSFILEREADER_HPP
