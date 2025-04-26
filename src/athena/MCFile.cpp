#include "athena/MCFile.hpp"

namespace athena {

const char MCFile::VERSION_EU_JP[33] = "AGBZELDA:THE MINISH CAP:ZELDA 3\0";
const char MCFile::VERSION_US[33] = "AGBZELDA:THE MINISH CAP:ZELDA 5\0";
MCFile::MCFile() {}

// TODO: Rewrite this to be more optimized, the current solution takes quite a few cycles
uint8_t* reverse(uint8_t* data, uint32_t length) {
  uint32_t a = 0;
  uint32_t swap;

  for (; a < --length; a++) {
    swap = data[a];
    data[a] = data[length];
    data[length] = swap;
  }

  return data;
}

uint8_t* MCFile::unscramble(uint8_t* data, uint64_t length) {
  if (!data)
    return nullptr;

  for (uint32_t i = 0; i < length; i += 8) {
    uint32_t block1 = *(uint32_t*)reverse((data + i), 4);
    uint32_t block2 = *(uint32_t*)reverse((data + i + 4), 4);
    *(uint32_t*)(data + i) = block2;
    *(uint32_t*)(data + i + 4) = block1;
  }

  return data;
}

} // namespace athena
