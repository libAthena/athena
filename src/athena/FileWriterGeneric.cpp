#include "athena/FileWriter.hpp"

namespace athena
{
namespace io
{
void TransactionalFileWriter::seek(atInt64 pos, SeekOrigin origin)
{
    switch (origin)
    {
    case SeekOrigin::Begin:
        m_position = pos;
        break;
    case SeekOrigin::Current:
        m_position += pos;
        break;
    case SeekOrigin::End:
        break;
    }
}

void TransactionalFileWriter::writeUBytes(const atUint8* data, atUint64 len)
{
    atUint64 neededSz = m_position + len;
    if (neededSz > m_deferredBuffer.size())
    {
        m_deferredBuffer.reserve(neededSz * 2);
        m_deferredBuffer.resize(neededSz);
    }

    memmove(m_deferredBuffer.data() + m_position, data, len);
    m_position += len;
}
}
}
