#include "test.hpp"
#include "athena/MemoryWriter.hpp"

#define EXPECTED_BYTES 265

int main(int argc, const char** argv)
{
    TESTFile file = {};
    file.arrCount[0] = 2;
    file.array.push_back(42);
    file.array.push_back(64);
    size_t binSize = 0;
    file.binarySize(binSize);
    athena::io::MemoryCopyWriter w(nullptr, binSize);
    atInt64 pos = w.position();
    file.write(w);
    bool pass = !w.hasError() && w.position() - pos == binSize && binSize == EXPECTED_BYTES;
    if (pass)
        printf("[PASS] %" PRISize " bytes written\n", size_t(w.position() - pos));
    else
        printf("[FAIL] %" PRISize " bytes written; %" PRISize " bytes sized; %d bytes expected\n",
               size_t(w.position() - pos), binSize, EXPECTED_BYTES);
    return pass ? 0 : 1;
}
