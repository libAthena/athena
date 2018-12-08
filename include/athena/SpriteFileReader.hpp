#pragma once

#include "athena/MemoryReader.hpp"

namespace athena::Sakura {
class SpriteFile;
} // namespace athena::Sakura

namespace athena {
namespace io {

class SpriteFileReader : public MemoryCopyReader {
public:
  SpriteFileReader(atUint8* data, atUint64 length);
  SpriteFileReader(const std::string& filepath);

  Sakura::SpriteFile* readFile();
};
} // namespace io
} // namespace athena
