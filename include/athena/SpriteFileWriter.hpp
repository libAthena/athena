#pragma once

#include "athena/MemoryWriter.hpp"

namespace athena {
namespace Sakura {
class SpriteFile;
} // namespace Sakura

namespace io {

class SpriteFileWriter : public MemoryCopyWriter {
public:
  SpriteFileWriter(atUint8* data, atUint64 length);

  SpriteFileWriter(std::string_view filepath);

  void writeFile(Sakura::SpriteFile* file);
};

} // namespace io
} // namespace athena
