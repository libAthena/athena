#pragma once

#include "athena/Types.hpp"
#include "athena/MemoryWriter.hpp"

namespace athena
{

class MCFile;

namespace io
{

/*! \class MCFileWriter
 *  \brief The Minish Cap Save save data writer class
 *
 *  A Class for writing binary data to a The Minish Cap Save File,
 *  all work is done using a memory buffer, and not written directly from the disk.
 *  \sa BinaryWriter
 */
class MCFileWriter : protected MemoryCopyWriter
{
public:
    /*!
     * \brief This constructor takes an existing buffer to write to.
     *
     * \param data The existing buffer
     * \param length The length of the existing buffer
     */
    MCFileWriter(atUint8*, atUint64);

    /*!
     * \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    MCFileWriter(const std::string&);

    /*!
     * \brief Writes the given save data to a file on disk
     *
     * \param file Save data to write
     */
    void writeFile(MCFile* file);

    static atUint16 calculateChecksum(atUint8* data, atUint32 length);
private:
    atUint16 calculateSlotChecksum(atUint32 game);
};

} // io
} // zelda

