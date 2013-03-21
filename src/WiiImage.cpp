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

#include "WiiImage.hpp"
#include <utility.hpp>
#include <string.h>
#include <stdlib.h>

WiiImage::WiiImage(Uint32 width, Uint32 height, Uint8* data) :
    m_width(width),
    m_height(height),
    m_data(data)
{
}

WiiImage::~WiiImage()
{
    if (m_data)
        delete[] m_data;
    m_data = NULL;
}

Uint8* WiiImage::data()
{
    return m_data;
}

Uint32 WiiImage::width() const
{
    return m_width;
}

Uint32 WiiImage::height() const
{
    return m_height;
}

Uint8 *WiiImage::toRGBA()
{
    Uint32 x, y;
    Uint32 x1, y1;
    Uint32 iv;
    Uint8* bitmapdata = NULL;

    bitmapdata = new Uint8[m_width * m_height * 4];
    if(bitmapdata == NULL)
            return NULL;

    for(iv = 0, y1 = 0; y1 < m_height; y1 += 4)
    {
        for(x1 = 0; x1 < m_width; x1 += 4)
        {
            for(y = y1; y < (y1 + 4); y++)
            {
                for(x = x1; x < (x1 + 4); x++)
                {
                    Uint16 oldpixel = *(Uint16*)(m_data + ((iv++) * 2));
                    //if((x >= m_width) || (y >= m_height))
                    //        continue;
                    oldpixel = swapU16(oldpixel);
                    if(oldpixel & (1 << 15))
                    {
                        // RGB5
                        Uint8 b = (((oldpixel >> 10) & 0x1F) * 255) / 31;
                        Uint8 g = (((oldpixel >> 5)  & 0x1F) * 255) / 31;
                        Uint8 r = (((oldpixel >> 0)  & 0x1F) * 255) / 31;
                        Uint8 a = 255;
                        Uint8 rgba = (r << 0) | (g << 8) | (b << 16) | (a << 24);
                        (*(Uint32**)&bitmapdata)[x + (y * m_width)] = rgba;
                    }
                    else
                    {
                        // RGB4A3
                        Uint8 a = (((oldpixel >> 12) & 0x7) * 255) / 7;
                        Uint8 b = (((oldpixel >> 8)  & 0xF) * 255) / 15;
                        Uint8 g = (((oldpixel >> 4)  & 0xF) * 255) / 15;
                        Uint8 r = (((oldpixel >> 0)  & 0xF) * 255) / 15;
                        Uint32 rgba = (r << 0) | (g << 8) | (b << 16) | (a << 24);
                        (*(Uint32**)&bitmapdata)[x + (y * m_width)] = rgba;
                    }
                }
            }
        }
    }
    return bitmapdata;
}
