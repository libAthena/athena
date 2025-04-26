#pragma once

#include "athena/MemoryReader.hpp"

namespace athena {
class SkywardSwordFile;
namespace io {
class SkywardSwordFileReader : public MemoryCopyReader {
public:
  SkywardSwordFileReader(uint8_t* data, uint64_t length);
  SkywardSwordFileReader(const std::string& filename);

  SkywardSwordFile* read();
};
} // namespace io
} // namespace athena
