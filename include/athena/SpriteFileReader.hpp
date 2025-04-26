#pragma once

#include "athena/MemoryReader.hpp"

namespace athena::Sakura {
class SpriteFile;
} // namespace athena::Sakura

namespace athena {
namespace io {

class SpriteFileReader : public MemoryCopyReader {
public:
  SpriteFileReader(uint8_t* data, uint64_t length);
  SpriteFileReader(const std::string& filepath);

  Sakura::SpriteFile* readFile();
};
} // namespace io
} // namespace athena
