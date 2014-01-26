#ifndef __SSFILEWRITER_HPP__
#define __SSFILEWRITER_HPP__

#include "BinaryWriter.hpp"

namespace zelda
{
class SkywardSwordFile;

namespace io
{

class SkywardSwordFileWriter : public BinaryWriter
{
    // Why does this fuck up my formatting in Qt Creator?
    BINARYWRITER_BASE
public:
    SkywardSwordFileWriter(Uint8* data, Uint64 len);
    SkywardSwordFileWriter(const std::string& filename);

    void write(SkywardSwordFile* file);
};
}
}
#endif // __SSFILEWRITER_HPP__
