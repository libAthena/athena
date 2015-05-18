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

#ifndef WIIFILE_H
#define WIIFILE_H

#include <string>
#include <map>
#include "Athena/Global.hpp"

namespace Athena
{

const atUint8 SD_KEY     [16] = {0xab, 0x01, 0xb9, 0xd8, 0xe1, 0x62, 0x2b, 0x08, 0xaf, 0xba, 0xd8, 0x4d, 0xbf, 0xc2, 0xa5, 0x5d};
const atUint8 SD_IV      [16] = {0x21, 0x67, 0x12, 0xe6, 0xaa, 0x1f, 0x68, 0x9f, 0x95, 0xc5, 0xa2, 0x23, 0x24, 0xdc, 0x6a, 0x98};
const atUint8 MD5_BLANKER[16] = {0x0e, 0x65, 0x37, 0x81, 0x99, 0xbe, 0x45, 0x17, 0xab, 0x06, 0xec, 0x22, 0x45, 0x1a, 0x57, 0x93};

/*! \class WiiFile
 *  \brief Wii file container class
 *
 * Contains all relevant data for a file in a data.bin file.
 */
class WiiFile
{
public:

    /*! \enum Permission
     *  \brief The Wii uses a bastardized unix permissions system so these flags
     *  reflect the file's individual permissions.
     */
    enum Permission
    {
        OtherRead  = 0x01,
        OtherWrite = 0x02,
        GroupRead  = 0x04,
        GroupWrite = 0x08,
        OwnerRead  = 0x10,
        OwnerWrite = 0x20,

        // Mask values;
        OtherRW = (OtherRead|OtherWrite), //!< Mask to get the Other group permissions
        GroupRW = (GroupRead|GroupWrite),
        OwnerRW = (OwnerRead|OwnerWrite)
    };

    /*!
     * \brief The Type enum
     */
    enum Type
    {
        File      = 0x01,
        Directory = 0x02
    };

    WiiFile();

    /*!
     * \brief WiiFile
     * \param filename
     */
    WiiFile(const std::string& filename);

    /*!
     * \brief WiiFile
     * \param filename
     * \param permissions
     * \param data
     * \param length
     */
    WiiFile(const std::string& filename, atUint8 permissions, const atUint8* data, atUint32 length);
    virtual ~WiiFile();

    /*!
     * \brief setFilename
     * \param filename
     */
    void     setFilename(const std::string& filename);

    /*!
     * \brief filename
     * \return
     */
    std::string filename() const;

    /*!
     * \brief setData
     * \param data
     */
    void setData(const atUint8* data);
    /*!
     * \brief data
     * \return
     */
    atUint8* data() const;

    /*!
     * \brief setLength
     * \param len
     */
    void setLength(const int len);

    /*!
     * \brief length
     * \return
     */
    int length() const;

    /*!
     * \brief setPermissions
     * \param permissions
     */
    void setPermissions(const atUint8 permissions);

    /*!
     * \brief permissions
     * \return
     */
    atUint8   permissions() const;

    /*!
     * \brief setAttributes
     * \param attr
     */
    void setAttributes(const atUint8 attr);

    /*!
     * \brief attributes
     * \return
     */
    atUint8   attributes() const;

    /*!
     * \brief setType
     * \param type
     */
    void setType(Type type);

    /*!
     * \brief type
     * \return
     */
    Type    type() const;

    /*!
     * \brief isDirectory
     * \return
     */
    bool isDirectory() const;

    /*!
     * \brief isFile
     * \return
     */
    bool isFile() const;

    /*!
     * \brief addChild
     * \param file
     */
    void addChild(WiiFile* file);
    /*!
     * \brief children
     * \return
     */
    std::vector<WiiFile*> children();
    /*!
     * \brief child
     * \param name
     * \return
     */
    WiiFile* child(const std::string& name);
    /*!
     * \brief removeChild
     * \param name
     */
    void removeChild(const std::string& name);
    /*!
     * \brief removeChild
     * \param file
     */
    void removeChild(WiiFile* file);

    /*!
     * \brief parent
     * \return
     */
    WiiFile* parent();

    /*!
     * \brief setParent
     * \param parent
     */
    void setParent(WiiFile* parent);

    /*!
     * \brief fileCount
     * \return
     */
    atUint32 fileCount();

    /*!
     * \brief allChildren
     * \return
     */
    std::vector<WiiFile*> allChildren();

    /*!
     * \brief fullpath
     * \return
     */
    std::string fullpath();
protected:
private:
    atUint8       m_permissions;
    atUint8       m_attributes;
    Type          m_type;
    std::string   m_filename;
    int           m_fileLen;
    atUint8*      m_fileData;
    WiiFile*     m_parent;
    std::vector<WiiFile*> m_children;
};

} // zelda
#endif // WIIFILE_H
#endif // ATHENA_NO_SAVES
