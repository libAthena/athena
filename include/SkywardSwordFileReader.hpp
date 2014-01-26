#ifndef __SSFILEREADER_HPP__
#define __SSFILEREADER_HPP__

#include "BinaryReader.hpp"

namespace zelda
{
class SkywardSwordFile;
namespace io
{
class SkywardSwordFileReader : public BinaryReader
{
    BINARYREADER_BASE
public:

    SkywardSwordFileReader(Uint8* data, Uint64 length);
    SkywardSwordFileReader(const std::string& filename);

    SkywardSwordFile* read();
};
} // io
} // zelda

#endif // __SSFILEREADER_HPP__
