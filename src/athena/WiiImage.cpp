#include "athena/WiiImage.hpp"
#include "athena/Utility.hpp"
#include <cstring>
#include <cstdlib>

namespace athena {

WiiImage::WiiImage(uint32_t width, uint32_t height, std::unique_ptr<uint8_t[]>&& data)
: m_width(width), m_height(height), m_data(std::move(data)) {}

uint8_t* WiiImage::data() { return m_data.get(); }

uint32_t WiiImage::width() const { return m_width; }

uint32_t WiiImage::height() const { return m_height; }

uint8_t* WiiImage::toRGBA() {
  uint32_t x, y;
  uint32_t x1, y1;
  uint32_t iv;
  uint8_t* bitmapdata = NULL;

  bitmapdata = new uint8_t[m_width * m_height * 4];

  if (bitmapdata == NULL)
    return NULL;

  for (iv = 0, y1 = 0; y1 < m_height; y1 += 4) {
    for (x1 = 0; x1 < m_width; x1 += 4) {
      for (y = y1; y < (y1 + 4); y++) {
        for (x = x1; x < (x1 + 4); x++) {
          uint16_t oldpixel = *(uint16_t*)(m_data.get() + ((iv++) * 2));
          // if((x >= m_width) || (y >= m_height))
          //        continue;
          oldpixel = utility::swapU16(oldpixel);

          if (oldpixel & (1 << 15)) {
            // RGB5
            uint8_t b = (((oldpixel >> 10) & 0x1F) * 255) / 31;
            uint8_t g = (((oldpixel >> 5) & 0x1F) * 255) / 31;
            uint8_t r = (((oldpixel >> 0) & 0x1F) * 255) / 31;
            uint8_t a = 255;
            uint8_t rgba = (r << 0) | (g << 8) | (b << 16) | (a << 24);
            (*(uint32_t**)&bitmapdata)[x + (y * m_width)] = rgba;
          } else {
            // RGB4A3
            uint8_t a = (((oldpixel >> 12) & 0x7) * 255) / 7;
            uint8_t b = (((oldpixel >> 8) & 0xF) * 255) / 15;
            uint8_t g = (((oldpixel >> 4) & 0xF) * 255) / 15;
            uint8_t r = (((oldpixel >> 0) & 0xF) * 255) / 15;
            uint32_t rgba = (r << 0) | (g << 8) | (b << 16) | (a << 24);
            (*(uint32_t**)&bitmapdata)[x + (y * m_width)] = rgba;
          }
        }
      }
    }
  }

  return bitmapdata;
}

} // namespace athena
