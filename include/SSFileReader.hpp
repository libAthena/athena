#ifndef __SSFILEREADER_HPP__
#define __SSFILEREADER_HPP__

#include "BinaryReader.hpp"

namespace zelda
{
class SSFile;
namespace io
{
class SSFileReader : public BinaryReader
{
    BINARYREADER_BASE
public:

    SSFileReader(Uint8* data, Uint64 length);
    SSFileReader(const std::string& filename);

    SSFile* read();
};
} // io
} // zelda

#endif // __SSFILEREADER_HPP__
