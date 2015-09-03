#ifndef MCFILEREADER_HPP
#define MCFILEREADER_HPP

#include "Athena/MemoryReader.hpp"

namespace Athena
{

class MCFile;

namespace io
{

/*! \class MCFileReader
 *  \brief The Minish Cap Save save data reader class
 *
 *  A Class for reading binary data from a The Minish Cap Save File,
 *  all work is done using a memory buffer, and not read directly from the disk.
 *  \sa BinaryReader
 */
class MCFileReader : public MemoryCopyReader
{
    MEMORYCOPYREADER_BASE();
public:
    /*!
     *  \brief This constructor takes an existing buffer to read from.
     *
     *  \param data The existing buffer
     *  \param length The length of the existing buffer
     */
    MCFileReader(atUint8*, atUint64);

    /*!
     *  \brief This constructor creates an instance from a file on disk.
     *
     *  \param filename The file to create the stream from
     */
    MCFileReader(const std::string&);

    /*!
     *  \brief Reads the save data from the buffer
     *
     *  \return MCFile* SRAM data
     */
    MCFile* readFile();
};

} // io
} // zelda

#endif // MCFILEREADER_HPP
