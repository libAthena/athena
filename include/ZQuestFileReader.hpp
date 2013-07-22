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

#ifndef __ZQUESTFILEREADER_HPP__
#define __ZQUESTFILEREADER_HPP__

#include "BinaryReader.hpp"

namespace zelda
{
class ZQuestFile;

namespace io
{

/*!
 * \brief The ZQuestFileReader class
 */
class ZQuestFileReader : protected BinaryReader
{
    BINARYREADER_BASE

public:    
    /*!
     * \brief ZQuestFileReader
     * \param data
     * \param length
     */
    ZQuestFileReader(Uint8* data, Uint64 length);

    /*!
     * \brief ZQuestFileReader
     * \param filename
     */
    ZQuestFileReader(const std::string& filename);

    /*!
     * \brief read
     * \return
     */
    ZQuestFile* read();
};

} // io
} // zelda

#endif // __ZQUESTFILEREADER_HPP__
