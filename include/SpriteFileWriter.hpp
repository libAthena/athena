#ifndef SSPRITEFILEWRITER_HPP
#define SSPRITEFILEWRITER_HPP

#include <BinaryWriter.hpp>

namespace zelda
{
namespace Sakura
{
class SpriteFile;
} // Sakura

namespace io
{

class SpriteFileWriter : public zelda::io::BinaryWriter
{
    BINARYWRITER_BASE;
public:
    SpriteFileWriter(Uint8* data, Uint64 length);

    SpriteFileWriter(const std::string& filepath);

    void writeFile(Sakura::SpriteFile* file);
};

} // io
} // zelda
#endif // SSPRITEFILEWRITER_HPP
