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
#ifndef __WII__SAVE_HPP__
#define __WII__SAVE_HPP__

#include <unordered_map>
#include <string>
#include <Types.hpp>

namespace zelda
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
    void     addFile(const std::string& filename, WiiFile* file);

    /*!
     * \brief file
     * \param filename
     * \return
     */
    WiiFile* file(const std::string& filename) const;
    /*!
     * \brief fileList
     * \return
     */
    std::unordered_map<std::string, WiiFile*>& fileList();

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

protected:
private:

    std::unordered_map<std::string, WiiFile*> m_files;
    WiiBanner* m_banner;

};

} // zelda
#endif // __WII__SAVE_HPP__
