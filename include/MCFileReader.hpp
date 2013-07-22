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

#ifndef __MCFILEREADER_HPP__
#define __MCFILEREADER_HPP__

#include "Types.hpp"
#include "BinaryReader.hpp"

namespace zelda
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
class MCFileReader : protected BinaryReader
{
    BINARYREADER_BASE
public:
    /*!
     *  \brief This constructor takes an existing buffer to read from.
     *
     *  \param data The existing buffer
     *  \param length The length of the existing buffer
     */
    MCFileReader(Uint8*, Uint64);

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

#endif // __MCFILEREADER_HPP__
