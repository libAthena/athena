#pragma once

#include "athena/MemoryReader.hpp"

namespace athena
{
class SkywardSwordFile;
namespace io
{
class SkywardSwordFileReader : public MemoryCopyReader
{
public:
    SkywardSwordFileReader(atUint8* data, atUint64 length);
    SkywardSwordFileReader(const std::string& filename);

    SkywardSwordFile* read();
};
} // io
} // zelda

