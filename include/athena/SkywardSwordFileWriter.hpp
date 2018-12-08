#pragma once

#include "athena/MemoryWriter.hpp"

namespace athena {
class SkywardSwordFile;

namespace io {

class SkywardSwordFileWriter : public MemoryCopyWriter {
public:
  SkywardSwordFileWriter(atUint8* data, atUint64 len);
  SkywardSwordFileWriter(const std::string& filename);

  void write(SkywardSwordFile* file);
};
} // namespace io
} // namespace athena
