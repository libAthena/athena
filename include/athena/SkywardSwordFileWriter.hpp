#pragma once

#include "athena/MemoryWriter.hpp"

namespace athena {
class SkywardSwordFile;

namespace io {

class SkywardSwordFileWriter : public MemoryCopyWriter {
public:
  SkywardSwordFileWriter(uint8_t* data, uint64_t len);
  SkywardSwordFileWriter(const std::string& filename);

  void write(SkywardSwordFile* file);
};
} // namespace io
} // namespace athena
