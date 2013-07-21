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

#ifndef __ZQUESTFILEWRITER_HPP__
#define __ZQUESTFILEWRITER_HPP__

#include <BinaryWriter.hpp>

namespace zelda
{
class ZQuest;

namespace io
{

/*!
 * \brief The ZQuestFileWriter class
 */
class ZQuestFileWriter : public io::BinaryWriter
{
    BINARYWRITER_BASE

public:
    /*!
     * \brief ZQuestFileWriter
     * \param data
     * \param length
     */
    ZQuestFileWriter(Uint8* data, Uint64 length);

    /*!
     * \brief ZQuestFileWriter
     * \param filename
     */
    ZQuestFileWriter(const std::string& filename);

    /*!
     * \brief write
     * \param quest
     * \param compress
     */
    void write(ZQuest* quest, bool compress = true);
};

} // io
} // zelda
#endif // __ZQUESTFILEWRITER_HPP__
