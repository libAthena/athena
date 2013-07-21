#ifndef ZQUESTFILEREADER_HPP
#define ZQUESTFILEREADER_HPP

#include "BinaryReader.hpp"

namespace zelda
{
class ZQuest;

class ZQuestFileReader : public io::BinaryReader
{
    BINARYREADER_BASE

public:
    ZQuestFileReader(Uint8* data, Uint64 length);
    ZQuestFileReader(const std::string& filename);

    ZQuest* read();
};
} // zelda

#endif // ZQUESTFILEREADER_HPP
