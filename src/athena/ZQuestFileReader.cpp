#include "athena/ZQuestFileReader.hpp"
#include "athena/ZQuestFile.hpp"
#include "athena/Compression.hpp"
#include "athena/Checksums.hpp"
#include "athena/Utility.hpp"

#include <iostream>
#include <iomanip>

namespace athena::io {

ZQuestFileReader::ZQuestFileReader(uint8_t* data, uint64_t length) : MemoryCopyReader(data, length) {}

ZQuestFileReader::ZQuestFileReader(const std::string& filename) : MemoryCopyReader(filename) {}

ZQuestFile* ZQuestFileReader::read() {
  uint32_t magic, version, compressedLen, uncompressedLen;
  ZQuestFile::Game game = ZQuestFile::NoGame;
  std::string gameString;
  uint16_t BOM;
  uint32_t checksum = 0;

  magic = readUint32();

  if ((magic & 0x00FFFFFF) != (ZQuestFile::Magic & 0x00FFFFFF)) {
    atError("Not a valid ZQuest file");
    return nullptr;
  }

  version = readUint32();

  if (version > ZQuestFile::Version) {
    atError("Unsupported ZQuest version");
    return nullptr;
  }

  compressedLen = readUint32();
  uncompressedLen = readUint32();

  if (version >= ZQUEST_VERSION_CHECK(2, 0, 0)) {
    gameString = std::string((const char*)readBytes(0x0A).get(), 0x0A);

    for (size_t i = 0; i < ZQuestFile::gameStringList().size(); i++) {
      if (ZQuestFile::gameStringList().at(i).substr(0, 0x0A) == gameString) {
        gameString = ZQuestFile::gameStringList().at(i);
        game = (ZQuestFile::Game)i;
        break;
      }
    }

    BOM = readUint16();
    checksum = readUint32();
  } else {
    game = (ZQuestFile::Game)readUint32();
    BOM = readUint16();
    seek(0x0A);
  }

  std::unique_ptr<uint8_t[]> data = readUBytes(compressedLen); // compressedLen is always the total file size

  if (version >= ZQUEST_VERSION_CHECK(2, 0, 0)) {
    if (checksum != athena::checksums::crc32(data.get(), compressedLen)) {
      atError("Checksum mismatch, data corrupt");
      return nullptr;
    }
  } else {
    std::clog << "ZQuest version 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex
              << athena::utility::swapU32(version);
    std::clog << " has no checksum field" << std::endl;
  }

  if (compressedLen != uncompressedLen) {
    uint8_t* dst = new uint8_t[uncompressedLen];
    uint32_t dstLen = io::Compression::decompressZlib(data.get(), compressedLen, dst, uncompressedLen);

    if (dstLen != uncompressedLen) {
      delete[] dst;
      atError("Error decompressing data");
      return nullptr;
    }

    data.reset(dst);
  }

  return new ZQuestFile(game, BOM == 0xFEFF ? Endian::Big : Endian::Little, std::move(data), uncompressedLen,
                        gameString);
}

} // namespace athena::io
