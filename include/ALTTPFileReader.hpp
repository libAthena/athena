// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>

#ifndef __ALTTP_FILE_READER_HPP__
#define __ALTTP_FILE_READER_HPP__

#include <string>
#include "Types.hpp"
#include "BinaryReader.hpp"
#include "ALTTPQuest.hpp"

namespace zelda
{
class ALTTPFile;

namespace io
{
/*! \class ALTTPFileReader
 *  \brief A Link to the Past save data reader class
 *
 *  A Class for reading binary data from an ALTTP Save File,
 *  all work is done using a memory buffer, and not read directly from the disk.
 *  \sa BinaryReader
 */
class ALTTPFileReader : protected BinaryReader
{
    BINARYREADER_BASE

public:
    /*! \brief This constructor takes an existing buffer to read from.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    ALTTPFileReader(Uint8*, Uint64);

    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    ALTTPFileReader(const std::string&);

    /*! \brief Reads the SRAM data from the buffer
     *
     * \return ALTTPFile* SRAM data
     */
    ALTTPFile* readFile();
private:
    ALTTPRoomFlags* readRoomFlags();
    ALTTPOverworldEvent* readOverworldEvent();
    ALTTPDungeonItemFlags readDungeonFlags();
};

} // io
} // zelda
#endif // __ALTTP_FILE_READER_HPP__
