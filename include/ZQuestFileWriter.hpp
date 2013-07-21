#ifndef ZQUESTFILEWRITER_HPP
#define ZQUESTFILEWRITER_HPP

#include <BinaryWriter.hpp>

namespace zelda
{
class ZQuest;

class ZQuestFileWriter : public io::BinaryWriter
{
    BINARYWRITER_BASE

public:
    ZQuestFileWriter(Uint8* data, Uint64 length);
    ZQuestFileWriter(const std::string& filename);

    void write(ZQuest* quest, bool compress = true);
};
}
#endif // ZQUESTFILEWRITER_HPP
