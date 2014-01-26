#ifndef SSPRITEFILEREADER_HPP
#define SSPRITEFILEREADER_HPP

#include <BinaryReader.hpp>

namespace zelda
{
namespace Sakura
{
class SpriteFile;
} // Sakura

namespace io
{

class SpriteFileReader : public zelda::io::BinaryReader
{
    BINARYREADER_BASE;
public:
    SpriteFileReader(Uint8* data, Uint64 length);
    SpriteFileReader(const std::string& filepath);

    Sakura::SpriteFile* readFile();
};
} // io
} // zelda


#endif // SSPRITEFILEREADER_HPP
