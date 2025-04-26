#pragma once

#include "athena/MemoryWriter.hpp"

namespace athena {
namespace Sakura {
class SpriteFile;
} // namespace Sakura

namespace io {

class SpriteFileWriter : public MemoryCopyWriter {
public:
  SpriteFileWriter(uint8_t* data, uint64_t length);

  SpriteFileWriter(std::string_view filepath);

  void writeFile(Sakura::SpriteFile* file);
};

} // namespace io
} // namespace athena
