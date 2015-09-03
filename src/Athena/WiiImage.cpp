#include "Athena/WiiImage.hpp"
#include "Athena/Utility.hpp"
#include <string.h>
#include <stdlib.h>

namespace Athena
{

WiiImage::WiiImage(atUint32 width, atUint32 height, std::unique_ptr<atUint8[]>&& data) :
    m_width(width),
    m_height(height),
    m_data(std::move(data))
{
}

atUint8* WiiImage::data()
{
    return m_data.get();
}

atUint32 WiiImage::width() const
{
    return m_width;
}

atUint32 WiiImage::height() const
{
    return m_height;
}

atUint8* WiiImage::toRGBA()
{
    atUint32 x, y;
    atUint32 x1, y1;
    atUint32 iv;
    atUint8* bitmapdata = NULL;

    bitmapdata = new atUint8[m_width * m_height * 4];

    if (bitmapdata == NULL)
        return NULL;

    for (iv = 0, y1 = 0; y1 < m_height; y1 += 4)
    {
        for (x1 = 0; x1 < m_width; x1 += 4)
        {
            for (y = y1; y < (y1 + 4); y++)
            {
                for (x = x1; x < (x1 + 4); x++)
                {
                    atUint16 oldpixel = *(atUint16*)(m_data.get() + ((iv++) * 2));
                    //if((x >= m_width) || (y >= m_height))
                    //        continue;
                    oldpixel = utility::swapU16(oldpixel);

                    if (oldpixel & (1 << 15))
                    {
                        // RGB5
                        atUint8 b = (((oldpixel >> 10) & 0x1F) * 255) / 31;
                        atUint8 g = (((oldpixel >> 5)  & 0x1F) * 255) / 31;
                        atUint8 r = (((oldpixel >> 0)  & 0x1F) * 255) / 31;
                        atUint8 a = 255;
                        atUint8 rgba = (r << 0) | (g << 8) | (b << 16) | (a << 24);
                        (*(atUint32**)&bitmapdata)[x + (y * m_width)] = rgba;
                    }
                    else
                    {
                        // RGB4A3
                        atUint8 a = (((oldpixel >> 12) & 0x7) * 255) / 7;
                        atUint8 b = (((oldpixel >> 8)  & 0xF) * 255) / 15;
                        atUint8 g = (((oldpixel >> 4)  & 0xF) * 255) / 15;
                        atUint8 r = (((oldpixel >> 0)  & 0xF) * 255) / 15;
                        atUint32 rgba = (r << 0) | (g << 8) | (b << 16) | (a << 24);
                        (*(atUint32**)&bitmapdata)[x + (y * m_width)] = rgba;
                    }
                }
            }
        }
    }

    return bitmapdata;
}

} // zelda
