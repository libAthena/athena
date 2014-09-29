#ifndef ATHENA_NO_SAVES
// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

#ifndef __MCFILEWRITER_HPP__
#define __MCFILEWRITER_HPP__

#include "Athena/Types.hpp"
#include "Athena/BinaryWriter.hpp"

namespace Athena
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
class MCFileWriter : protected BinaryWriter
{
    BINARYWRITER_BASE
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

private:
    atUint16 calculateSlotChecksum(atUint32 game);
    atUint16 calculateChecksum(atUint8* data, atUint32 length);
    atUint8* reverse(atUint8* data, atUint32 length);
    void unscramble();
};

} // io
} // zelda

#endif // __MCFILEWRITER_HPP__
#endif // ATHENA_NO_SAVES
