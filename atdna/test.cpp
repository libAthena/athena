#include "test.hpp"

#include <athena/MemoryWriter.hpp>
#include <fmt/format.h>

#define EXPECTED_BYTES 281

int main(int argc, const char** argv) {
  TESTFile<atUint32, 2> file = {};
  file.arrCount[0] = 2;
  file.array.push_back(42);
  file.array.push_back(64);
  size_t binSize = 0;
  file.binarySize(binSize);
  athena::io::MemoryCopyWriter w(nullptr, binSize);
  atInt64 pos = w.position();
  file.write(w);

  const bool pass = !w.hasError() && w.position() - pos == binSize && binSize == EXPECTED_BYTES;
  if (pass) {
    fmt::print(fmt("[PASS] {} bytes written\n"), size_t(w.position() - pos));
  } else {
    fmt::print(fmt("[FAIL] {} bytes written; {} bytes sized; {} bytes expected\n"), size_t(w.position() - pos), binSize,
               EXPECTED_BYTES);
  }

  return pass ? 0 : 1;
}
