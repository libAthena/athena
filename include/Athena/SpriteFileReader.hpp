#ifndef SSPRITEFILEREADER_HPP
#define SSPRITEFILEREADER_HPP

#include "Athena/MemoryReader.hpp"

namespace Athena
{
namespace Sakura
{
class SpriteFile;
} // Sakura

namespace io
{

class SpriteFileReader : public MemoryCopyReader
{
    MEMORYCOPYREADER_BASE();
public:
    SpriteFileReader(atUint8* data, atUint64 length);
    SpriteFileReader(const std::string& filepath);

    Sakura::SpriteFile* readFile();
};
} // io
} // zelda


#endif // SSPRITEFILEREADER_HPP
