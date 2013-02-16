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

class WiiFile
{
public:
    enum Permission
    {
        OtherRead  = 0x01,
        OtherWrite = 0x02,
        GroupRead  = 0x04,
        GroupWrite = 0x08,
        OwnerRead  = 0x10,
        OwnerWrite = 0x20,

        // Mask values;
        OtherRW = (OtherRead|OtherWrite),
        GroupRW = (GroupRead|GroupWrite),
        OwnerRW = (OwnerRead|OwnerWrite)
    };

    enum Type
    {
        File      = 0x01,
        Directory = 0x02
    };

    WiiFile();
    WiiFile(const std::string& filename);
    WiiFile(const std::string& filename, Uint8 permissions, const Uint8* data, Uint32 length);
    virtual ~WiiFile();

    void     setFilename(const std::string& filename);
    std::string filename() const;

    void setData(const Uint8* data);
    Uint8*  data() const;

    void setLength(const int len);
    int     length() const;

    void setPermissions(const Uint8 permissions);
    Uint8   permissions() const;

    void setAttributes(const Uint8 attr);
    Uint8   attributes() const;

    void setType(Type type);
    Type    type() const;

    bool isDirectory() const;
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

#endif // WIIFILE_H
