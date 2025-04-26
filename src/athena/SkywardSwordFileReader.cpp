#include "athena/SkywardSwordFileReader.hpp"
#include "athena/SkywardSwordFile.hpp"
#include "athena/SkywardSwordQuest.hpp"
#include <iostream>

namespace athena::io {

SkywardSwordFileReader::SkywardSwordFileReader(uint8_t* data, uint64_t length) : MemoryCopyReader(data, length) {
  setEndian(Endian::Big);
}

SkywardSwordFileReader::SkywardSwordFileReader(const std::string& filename) : MemoryCopyReader(filename) {
  setEndian(Endian::Big);
}

SkywardSwordFile* SkywardSwordFileReader::read() {
  SkywardSwordFile* file = NULL;

  if (length() != 0xFBE0) {
    atError("File not the expected size of 0xFBE0");
    return nullptr;
  }

  uint32_t magic = readUint32();

  if (magic != SkywardSwordFile::USMagic && magic != SkywardSwordFile::JAMagic && magic != SkywardSwordFile::EUMagic) {
    atError("Not a valid Skyward Sword save file");
    return nullptr;
  }

  seek(0x01C, SeekOrigin::Begin);
  uint32_t headerSize = readUint32(); // Seems to be (headerSize - 1)

  if (headerSize != 0x1D) {
    atError("Invalid header size, Corrupted data?");
    return nullptr;
  }

  // Time to read in each slot
  file = new SkywardSwordFile;
  file->setRegion((magic == SkywardSwordFile::USMagic
                       ? Region::NTSC
                       : (magic == SkywardSwordFile::JAMagic ? Region::NTSCJ : Region::PAL)));

  for (int i = 0; i < 3; i++) {
    SkywardSwordQuest* q = new SkywardSwordQuest(readUBytes(0x53C0), 0x53C0);
    uint64_t pos = position();
    // seek to the skip data for this particular quest
    seek(0xFB60 + (i * 0x24), SeekOrigin::Begin);
    q->setSkipData(readUBytes(0x24));
    seek(pos, SeekOrigin::Begin);
    file->addQuest(q);
  }

  return file;
}

} // namespace athena::io
