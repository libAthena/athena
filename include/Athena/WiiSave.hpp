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

#ifndef WIISAVE_HPP
#define WIISAVE_HPP

#include <unordered_map>
#include <string>
#include "Athena/Global.hpp"

namespace Athena
{

class WiiFile;
class WiiBanner;
class WiiImage;
class BinaryReader;
class BinaryWriter;

/*! \class WiiSave
 *  \brief Wii data.bin container class
 *
 * Contains all relevant data for a Wii data.bin file.
 */
class WiiSave
{
public:

    /*!
     * \brief FileIterator
     */
    typedef std::unordered_map<std::string, WiiFile*>::const_iterator FileIterator;

    /*!
     * \brief WiiSave
     */
    WiiSave();
    /*!
     * \brief ~WiiSave
     */
    virtual ~WiiSave();

    /*!
     * \brief addFile
     * \param filename
     * \param file
     */
    void     addFile(WiiFile* file);
    void setRoot(WiiFile* root);
    /*!
     * \brief file
     * \param filename
     * \return
     */
    WiiFile* file(const std::string& filename);

    atUint32 fileCount() const;
    /*!
     * \brief fileList
     * \return
     */
    WiiFile* root();

    /*!
     * \brief setBanner
     * \param banner
     */
    void     setBanner(WiiBanner* banner);

    /*!
     * \brief banner
     * \return
     */
    WiiBanner* banner() const;

    std::vector<WiiFile*> allFiles() const;

protected:
private:

    WiiFile*   m_root;
    WiiBanner* m_banner;

};

} // zelda
#endif // WIISAVE_HPP
#endif // ATHENA_NO_SAVES
