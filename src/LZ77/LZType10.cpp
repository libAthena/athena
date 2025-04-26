#include "LZ77/LZType10.hpp"

#include <cstddef>
#include <cstring>
#include <memory>

#include "LZ77/LZLookupTable.hpp"

#include <athena/MemoryWriter.hpp>

LZType10::LZType10(int32_t MinimumOffset, int32_t SlidingWindow, int32_t MinimumMatch, int32_t BlockSize)
: LZBase(MinimumOffset, SlidingWindow, MinimumMatch, BlockSize) {
  // ReadAheadBuffer is normalize between (minumum match) and(minimum match + 15) so that matches fit within
  // 4-bits.
  m_readAheadBuffer = m_minMatch + 0xF;
}

uint32_t LZType10::compress(const uint8_t* src, uint8_t** dstBuf, uint32_t srcLength) {
  uint32_t encodeSize = (srcLength << 8) | (0x10);
  encodeSize = athena::utility::LittleUint32(encodeSize); // File size needs to be written as little endian always

  athena::io::MemoryCopyWriter outbuf("tmp");
  outbuf.writeUint32(encodeSize);

  const uint8_t* ptrStart = src;
  const uint8_t* ptrEnd = src + srcLength;

  // At most their will be two bytes written if the bytes can be compressed. So if all bytes in the block can be
  // compressed it would take blockSize*2 bytes
  auto compressedBytes = std::unique_ptr<uint8_t[]>(new uint8_t[m_blockSize * 2]); // Holds the compressed bytes yet to be written

  while (ptrStart < ptrEnd) {
    uint8_t blockLen = 0;
    // In Binary represents 1 if byte is compressed or 0 if not compressed
    // For example 01001000 means that the second and fifth byte in the blockSize from the left is compressed
    uint8_t* ptrBytes = compressedBytes.get();

    for (int32_t i = 0; i < m_blockSize; i++) {
      // length_offset searchResult=Search(ptrStart, filedata, ptrEnd);
      const LZLengthOffset searchResult = m_lookupTable.search(ptrStart, src, ptrEnd);

      // If the number of bytes to be compressed is at least the size of the Minimum match
      if (searchResult.length >= static_cast<uint32_t>(m_minMatch)) {
        // Gotta swap the bytes since system is wii is big endian and most computers are little endian
        uint16_t lenOff = (((searchResult.length - m_minMatch) & 0xF) << 12) | ((searchResult.offset - 1) & 0xFFF);
        athena::utility::BigUint16(lenOff);

        memcpy(ptrBytes, &lenOff, sizeof(uint16_t));

        ptrBytes += sizeof(uint16_t);

        ptrStart += searchResult.length;

        blockLen |= (1 << (7 - i));
        // Stores which of the next 8 bytes is compressed
        // bit 1 for compress and bit 0 for not compressed
      } else {
        *ptrBytes++ = *ptrStart++;
      }
    }

    outbuf.writeByte(blockLen);
    outbuf.writeUBytes(compressedBytes.get(), static_cast<uint64_t>(ptrBytes - compressedBytes.get()));
  }

  // Add zeros until the file is a multiple of 4
  while ((outbuf.position() % 4) != 0) {
    outbuf.writeByte(0);
  }

  *dstBuf = outbuf.data();
  outbuf.save();
  return static_cast<uint32_t>(outbuf.length());
}

uint32_t LZType10::decompress(const uint8_t* src, uint8_t** dst, uint32_t srcLength) {
  if (*src != 0x10) {
    return 0;
  }

  // Size of data when it is uncompressed
  uint32_t uncompressedSize;
  std::memcpy(&uncompressedSize, src, sizeof(uncompressedSize));

  // The compressed file has the filesize encoded in little endian
  athena::utility::LittleUint32(uncompressedSize);

  // first byte is the encode flag
  uncompressedSize = uncompressedSize >> 8;

  auto uncompressedData = std::unique_ptr<uint8_t[]>(new uint8_t[uncompressedSize]);
  uint8_t* outputPtr = uncompressedData.get();
  uint8_t* outputEndPtr = uncompressedData.get() + uncompressedSize;
  const uint8_t* inputPtr = src + 4;
  const uint8_t* inputEndPtr = src + srcLength;

  while (inputPtr < inputEndPtr && outputPtr < outputEndPtr) {
    const uint8_t isCompressed = *inputPtr++;

    for (uint32_t i = 0; i < static_cast<uint32_t>(m_blockSize); i++) {
      // Checks to see if the next byte is compressed by looking
      // at its binary representation - E.g 10010000
      // This says that the first extracted byte and the four extracted byte is compressed
      if ((isCompressed >> (7 - i)) & 0x1) {
        uint16_t lenOff;
        memcpy(&lenOff, inputPtr, sizeof(uint16_t));
        athena::utility::BigUint16(lenOff);
        inputPtr += sizeof(uint16_t); // Move forward two bytes
        // length offset pair has been decoded.
        LZLengthOffset decoding;
        decoding.length = (lenOff >> 12) + m_minMatch;
        decoding.offset = static_cast<uint16_t>((lenOff & 0xFFF) + 1);

        if ((outputPtr - decoding.offset) < uncompressedData.get()) {
          // If the offset to look for uncompressed is passed the current uncompresed data then the data is not
          // compressed
          dst = nullptr;
          return 0;
        }

        for (size_t j = 0; j < decoding.length; ++j) {
          outputPtr[j] = (outputPtr - decoding.offset)[j];
        }

        outputPtr += decoding.length;
      } else {
        *outputPtr++ = *inputPtr++;
      }

      if (!(inputPtr < inputEndPtr && outputPtr < outputEndPtr)) {
        break;
      }
    }
  }

  *dst = uncompressedData.release();

  return uncompressedSize;
}
