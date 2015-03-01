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

#ifndef WIIIMAGE_HPP
#define WIIIMAGE_HPP

#include "Athena/Types.hpp"

namespace Athena
{

/*!
 * \brief The WiiImage class
 */
class WiiImage
{
public:
    /*!
     * \brief WiiImage
     */
    WiiImage();
    /*!
     * \brief WiiImage
     * \param width
     * \param height
     * \param data
     */
    WiiImage(atUint32 width, atUint32 height, atUint8* data);

    /*!
     * \brief ~WiiImage
     */
    ~WiiImage();

    /*!
     * \brief setWidth
     * \param width
     */
    void setWidth(const atUint32 width);

    /*!
     * \brief width
     * \return
     */
    atUint32  width() const;

    /*!
     * \brief setHeight
     * \param height
     */
    void  setHeight(const atUint32 height);

    /*!
     * \brief height
     * \return
     */
    atUint32   height() const;

    /*!
     * \brief setData
     * \param data
     */
    void  setData(const atUint8* data);

    /*!
     * \brief data
     * \return
     */
    atUint8*   data();

    /*!
     * \brief toRGBA
     * \return
     */
    atUint8* toRGBA();

private:
    atUint32 m_width;
    atUint32 m_height;
    atUint8* m_data;
};

} // zelda

#endif // WIIIMAGE_HPP
#endif // ATHENA_NO_SAVES
