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

#ifndef ALTTP_FILE_WRITER_HPP
#define ALTTP_FILE_WRITER_HPP

#include <string>
#include "Athena/MemoryWriter.hpp"
#include "Athena/ALTTPQuest.hpp"

namespace Athena
{
class ALTTPFile;

namespace io
{
/*! \class ALTTPFileWriter
 *  \brief A Link to the Past save data writer class
 *
 *  A Class for writing binary data to an ALTTP Save File,
 *  all work is done using a memory buffer, and not written directly to the disk.
 *  \sa BinaryReader
 */
class ALTTPFileWriter : protected MemoryCopyWriter
{
    MEMORYCOPYWRITER_BASE();

public:
    /*! \brief This constructor takes an existing buffer to write to.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    ALTTPFileWriter(atUint8*, atUint64);

    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    ALTTPFileWriter(const std::string&);

    /*! \brief Writes the given SRAM data to a file on disk
     *
     * \param file SRAM data to right
     */
    void writeFile(ALTTPFile* file);

private:
    void writeRoomFlags(ALTTPRoomFlags*);
    void writeOverworldEvent(ALTTPOverworldEvent*);
    void writeDungeonItems(ALTTPDungeonItemFlags);
    atUint16 calculateChecksum(atUint32 game);
};

} // io
} // zelda

#endif // ALTTP_FILE_WRITER_HPP
#endif // ATHENA_NO_SAVES
