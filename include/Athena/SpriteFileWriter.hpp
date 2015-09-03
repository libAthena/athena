#ifndef SSPRITEFILEWRITER_HPP
#define SSPRITEFILEWRITER_HPP

#include "Athena/MemoryWriter.hpp"

namespace Athena
{
namespace Sakura
{
class SpriteFile;
} // Sakura

namespace io
{

class SpriteFileWriter : public MemoryCopyWriter
{
    MEMORYCOPYWRITER_BASE();
public:
    SpriteFileWriter(atUint8* data, atUint64 length);

    SpriteFileWriter(const std::string& filepath);

    void writeFile(Sakura::SpriteFile* file);
};

} // io
} // zelda
#endif // SSPRITEFILEWRITER_HPP
