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
#ifndef WIIFILE_H
#define WIIFILE_H

#include <string>
#include <map>
#include <Types.hpp>

namespace zelda
{

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
    WiiFile(const std::string& filename, Uint8 permissions, const Uint8* data, Uint32 length);
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
    void setData(const Uint8* data);
    /*!
     * \brief data
     * \return
     */
    Uint8* data() const;

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
    void setPermissions(const Uint8 permissions);

    /*!
     * \brief permissions
     * \return
     */
    Uint8   permissions() const;

    /*!
     * \brief setAttributes
     * \param attr
     */
    void setAttributes(const Uint8 attr);

    /*!
     * \brief attributes
     * \return
     */
    Uint8   attributes() const;

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

protected:
private:
    Uint8       m_permissions;
    Uint8       m_attributes;
    Type        m_type;
    std::string m_filename;
    int         m_fileLen;
    Uint8*      m_fileData;
};

} // zelda
#endif // WIIFILE_H
