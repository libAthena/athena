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

#ifndef WIISAVEREADER_HPP
#define WIISAVEREADER_HPP

#include "Athena/Global.hpp"
#include "Athena/MemoryReader.hpp"

namespace Athena
{
class WiiSave;
class WiiBanner;
class WiiFile;
class WiiImage;

namespace io
{

/*! \class WiiSaveReader
 *  \brief Wii data.bin reader class
 *
 *  A Class for reading binary data from a wii data.bin file,
 *  all work is done using a memory buffer, and not read directly from the disk.
 *  \sa BinaryReader
 */
class WiiSaveReader : protected MemoryCopyReader
{
    MEMORYCOPYREADER_BASE();
public:
    /*! \brief This constructor takes an existing buffer to read from.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    WiiSaveReader(const atUint8*, atUint64);

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
    WiiImage*  readImage(atUint32 width, atUint32 height);
    void       readCerts(atUint32 totalSize);
    WiiFile*   buildTree(std::vector<WiiFile*> files);
};

} // io
} // zelda
#endif // WIISAVEREADER_HPP
#endif // ATHENA_NO_SAVES
