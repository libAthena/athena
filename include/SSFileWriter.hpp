#ifndef __SSFILEWRITER_HPP__
#define __SSFILEWRITER_HPP__

#include "BinaryWriter.hpp"

namespace zelda
{
class SSFile;

namespace io
{

class SSFileWriter : public BinaryWriter
{
    // Why does this fuck up my formatting in Qt Creator?
    BINARYWRITER_BASE
public:
    SSFileWriter(Uint8* data, Uint64 len);
    SSFileWriter(const std::string& filename);

    void write(SSFile* file);
};
}
}
#endif // __SSFILEWRITER_HPP__
