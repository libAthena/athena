#include "athena/ZQuestFileReader.hpp"
#include "athena/ZQuestFile.hpp"
#include "athena/Compression.hpp"
#include "athena/Checksums.hpp"
#include "athena/Utility.hpp"

#include <iostream>
#include <iomanip>

namespace athena::io {

ZQuestFileReader::ZQuestFileReader(atUint8* data, atUint64 length) : MemoryCopyReader(data, length) {}

ZQuestFileReader::ZQuestFileReader(const std::string& filename) : MemoryCopyReader(filename) {}

ZQuestFile* ZQuestFileReader::read() {
  atUint32 magic, version, compressedLen, uncompressedLen;
  ZQuestFile::Game game = ZQuestFile::NoGame;
  std::string gameString;
  atUint16 BOM;
  atUint32 checksum = 0;

  magic = readUint32();

  if ((magic & 0x00FFFFFF) != (ZQuestFile::Magic & 0x00FFFFFF)) {
    atError(fmt("Not a valid ZQuest file"));
    return nullptr;
  }

  version = readUint32();

  if (version > ZQuestFile::Version) {
    atError(fmt("Unsupported ZQuest version"));
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

  std::unique_ptr<atUint8[]> data = readUBytes(compressedLen); // compressedLen is always the total file size

  if (version >= ZQUEST_VERSION_CHECK(2, 0, 0)) {
    if (checksum != athena::checksums::crc32(data.get(), compressedLen)) {
      atError(fmt("Checksum mismatch, data corrupt"));
      return nullptr;
    }
  } else {
    std::clog << "ZQuest version 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex
              << athena::utility::swapU32(version);
    std::clog << " has no checksum field" << std::endl;
  }

  if (compressedLen != uncompressedLen) {
    atUint8* dst = new atUint8[uncompressedLen];
    atUint32 dstLen = io::Compression::decompressZlib(data.get(), compressedLen, dst, uncompressedLen);

    if (dstLen != uncompressedLen) {
      delete[] dst;
      atError(fmt("Error decompressing data"));
      return nullptr;
    }

    data.reset(dst);
  }

  return new ZQuestFile(game, BOM == 0xFEFF ? Endian::Big : Endian::Little, std::move(data), uncompressedLen,
                        gameString);
}

} // namespace athena::io
