#ifndef SSPRITEFILEREADER_HPP
#define SSPRITEFILEREADER_HPP

#include "athena/MemoryReader.hpp"

namespace athena::Sakura
{
class SpriteFile;
} // Sakura

namespace io
{

class SpriteFileReader : public MemoryCopyReader
{
public:
    SpriteFileReader(atUint8* data, atUint64 length);
    SpriteFileReader(const std::string& filepath);

    Sakura::SpriteFile* readFile();
};
} // zelda


#endif // SSPRITEFILEREADER_HPP
