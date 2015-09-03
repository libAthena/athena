#ifndef SSFILEWRITER_HPP
#define SSFILEWRITER_HPP

#include "Athena/MemoryWriter.hpp"

namespace Athena
{
class SkywardSwordFile;

namespace io
{

class SkywardSwordFileWriter : public MemoryCopyWriter
{
    MEMORYCOPYWRITER_BASE();
public:
    SkywardSwordFileWriter(atUint8* data, atUint64 len);
    SkywardSwordFileWriter(const std::string& filename);

    void write(SkywardSwordFile* file);
};
}
}
#endif // SSFILEWRITER_HPP
