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
#ifndef __WII_SAVE_READER_HPP__
#define __WII_SAVE_READER_HPP__

#include <Types.hpp>
#include <utility.hpp>
#include <BinaryReader.hpp>

namespace zelda
{

class WiiSave;
class WiiBanner;
class WiiFile;
class WiiImage;

/*! \class WiiSaveReader
 *  \brief Wii data.bin reader class
 *
 *  A Class for reading binary data from a wii data.bin file,
 *  all work is done using a memory buffer, and not read directly from the disk.
 *  \sa BinaryReader
 */
class WiiSaveReader : public io::BinaryReader
{
public:
    /*! \brief This constructor takes an existing buffer to read from.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    WiiSaveReader(const Uint8*, Uint64);

    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    WiiSaveReader(const std::string&);

    /*!
     * \brief readSave
     * \return
     */
    WiiSave* readSave();
private:
    WiiBanner* readBanner();
    WiiFile*   readFile();
    WiiImage*  readImage(Uint32 width, Uint32 height);
    void       readCerts(Uint32 totalSize);
};

} // zelda
#endif // __WII_SAVE_READER_HPP__
