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

#ifndef __WIIIMAGE_HPP__
#define __WIIIMAGE_HPP__

#include "Types.hpp"

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
    WiiImage(Uint32 width, Uint32 height, Uint8* data);

    /*!
     * \brief ~WiiImage
     */
    ~WiiImage();

    /*!
     * \brief setWidth
     * \param width
     */
    void setWidth(const Uint32 width);

    /*!
     * \brief width
     * \return
     */
    Uint32  width() const;

    /*!
     * \brief setHeight
     * \param height
     */
    void  setHeight(const Uint32 height);

    /*!
     * \brief height
     * \return
     */
    Uint32   height() const;

    /*!
     * \brief setData
     * \param data
     */
    void  setData(const Uint8* data);

    /*!
     * \brief data
     * \return
     */
    Uint8*   data();

    /*!
     * \brief toRGBA
     * \return
     */
    Uint8* toRGBA();

private:
    Uint32 m_width;
    Uint32 m_height;
    Uint8* m_data;
};

} // zelda

#endif // WIIIMAGE_HPP
