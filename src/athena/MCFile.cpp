#include "athena/MCFile.hpp"

namespace athena {

const char MCFile::VERSION_EU_JP[33] = "AGBZELDA:THE MINISH CAP:ZELDA 3\0";
const char MCFile::VERSION_US[33] = "AGBZELDA:THE MINISH CAP:ZELDA 5\0";
MCFile::MCFile() {}

// TODO: Rewrite this to be more optimized, the current solution takes quite a few cycles
atUint8* reverse(atUint8* data, atUint32 length) {
  atUint32 a = 0;
  atUint32 swap;

  for (; a < --length; a++) {
    swap = data[a];
    data[a] = data[length];
    data[length] = swap;
  }

  return data;
}

atUint8* MCFile::unscramble(atUint8* data, atUint64 length) {
  if (!data)
    return nullptr;

  for (atUint32 i = 0; i < length; i += 8) {
    atUint32 block1 = *(atUint32*)reverse((data + i), 4);
    atUint32 block2 = *(atUint32*)reverse((data + i + 4), 4);
    *(atUint32*)(data + i) = block2;
    *(atUint32*)(data + i + 4) = block1;
  }

  return data;
}

} // namespace athena
