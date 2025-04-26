#include "LZ77/LZType11.hpp"

#include <cstddef>
#include <cstring>
#include <memory>

#include "LZ77/LZLookupTable.hpp"

#include <athena/MemoryWriter.hpp>

LZType11::LZType11(int32_t minimumOffset, int32_t slidingWindow, int32_t minimumMatch, int32_t blockSize)
: LZBase(minimumOffset, slidingWindow, minimumMatch, blockSize) {
  m_readAheadBuffer = (0xF + 0xFF + 0xFFFF + m_minMatch);
  m_lookupTable.setLookAheadWindow(m_readAheadBuffer);
}

uint32_t LZType11::compress(const uint8_t* src, uint8_t** dst, uint32_t srcLength) {
  athena::io::MemoryCopyWriter outbuff("tmp");

  if (srcLength > 0xFFFFFF) { // If length is greater than 24 bits or 16 Megs
    uint32_t encodeFlag = 0x11;
    athena::utility::LittleUint32(encodeFlag);
    athena::utility::LittleUint32(srcLength); // Filesize data is little endian
    outbuff.writeUint32(encodeFlag);
    outbuff.writeUint32(srcLength);
  } else {
    uint32_t encodeSize = (srcLength << 8) | (0x11);
    athena::utility::LittleUint32(encodeSize);
    outbuff.writeUint32(encodeSize);
  }

  const uint8_t* ptrStart = src;
  const uint8_t* ptrEnd = src + srcLength;

  // At most their will be two bytes written if the bytes can be compressed. So if all bytes in the block can be
  // compressed it would take blockSize*2 bytes

  // Holds the compressed bytes yet to be written
  auto compressedBytes = std::unique_ptr<uint8_t[]>(new uint8_t[m_blockSize * 2]);

  const uint8_t maxTwoByteMatch = 0xF + 1;
  const uint8_t minThreeByteMatch = maxTwoByteMatch + 1; // Minimum Three byte match is maximum TwoByte match + 1
  const uint16_t maxThreeByteMatch = 0xFF + minThreeByteMatch;
  const uint16_t minFourByteMatch = maxThreeByteMatch + 1; // Minimum Four byte match is maximum Three Byte match + 1
  const int32_t maxFourByteMatch = 0xFFFF + minFourByteMatch;

  /*
  Normaliazation Example: If MIN_MATCH is 3 then 3 gets mapped to 2 and 16 gets mapped to 15.
  17 gets mapped to 1 and 272 gets mapped to 255
  273 gets mapped to 0 and 65808 gets mapped to 65535


  A two byte match uses 4 bits
  A three byte match uses 8 bits
  A four byte match uses 16 bits
  In each case the offset uses 12 bits
  In the two byte case the length is normalized so that the first 4 bits are numbers between between 2 and 15
  In the three byte case the first 4 bits are 0000
  In the four byte case the first 4 bits a 0001
  */
  while (ptrStart < ptrEnd) {
    uint8_t blockSize = 0;
    // In Binary represents 1 if byte is compressed or 0 if not compressed
    // For example 01001000 means that the second and fifth byte in the blockSize from the left is compressed
    uint8_t* ptrBytes = compressedBytes.get();

    for (int32_t i = 0; i < m_blockSize; i++) {
      // length_offset searchResult=Search(filedata,ptrStart,ptrEnd);
      const LZLengthOffset searchResult = m_lookupTable.search(ptrStart, src, ptrEnd);

      // If the number of bytes to be compressed is at least the size of the Minimum match
      if (searchResult.length >= static_cast<uint32_t>(m_minMatch)) {
        // Gotta swap the bytes since system is wii is big endian and most computers are little endian

        if (searchResult.length <= maxTwoByteMatch) {
          uint16_t lenOff = ((((searchResult.length - 1) & 0xF) << 12) | // Bits 15-12
                             ((searchResult.offset - 1) & 0xFFF)         // Bits 11-0
          );
          athena::utility::BigUint16(lenOff);
          memcpy(ptrBytes, &lenOff, 2);
          ptrBytes += 2;
        } else if (searchResult.length <= maxThreeByteMatch) {
          uint32_t lenOff = ((((searchResult.length - minThreeByteMatch) & 0xFF) << 12) | // Bits 20-12
                             ((searchResult.offset - 1) & 0xFFF)                          // Bits 11-0
          );
          athena::utility::BigUint32(lenOff);
          memcpy(ptrBytes, reinterpret_cast<uint8_t*>(&lenOff) + 1,
                 3); // Make sure to copy the lower 24 bits. 0x12345678- This statement copies 0x123456
          ptrBytes += 3;
        } else if (searchResult.length <= static_cast<uint32_t>(maxFourByteMatch)) {
          uint32_t lenOff = ((1 << 28) | // Bits 31-28 Flag to say that this is four bytes
                             (((searchResult.length - minFourByteMatch) & 0xFFFF) << 12) | // Bits 28-12
                             ((searchResult.offset - 1) & 0xFFF)                           // Bits 11-0
          );
          athena::utility::BigUint32(lenOff);
          memcpy(ptrBytes, &lenOff, 4);
          ptrBytes += 4;
        }

        ptrStart += searchResult.length;

        blockSize |= (1 << (7 - i));
        // Stores which of the next 8 bytes is compressed
        // bit 1 for compress and bit 0 for not compressed
      } else {
        *ptrBytes++ = *ptrStart++;
      }
    }

    outbuff.writeByte(blockSize);
    outbuff.writeUBytes(compressedBytes.get(), static_cast<uint64_t>(ptrBytes - compressedBytes.get()));
  }

  // Add zeros until the file is a multiple of 4
  while ((outbuff.position() % 4) != 0) {
    outbuff.writeByte(0);
  }

  *dst = outbuff.data();
  return static_cast<uint32_t>(outbuff.length());
}

uint32_t LZType11::decompress(const uint8_t* src, uint8_t** dst, uint32_t srcLength) {
  if (*src != 0x11) {
    return 0;
  }

  uint32_t uncompressedLen;
  std::memcpy(&uncompressedLen, src, sizeof(uncompressedLen));
  athena::utility::LittleUint32(uncompressedLen); // The compressed file has the filesize encoded in little endian
  uncompressedLen = uncompressedLen >> 8;         // First byte is the encode flag
  uint32_t currentOffset = 4;

  // If the filesize var is zero then the true filesize is over 14MB and must be read in from the next 4 bytes
  if (uncompressedLen == 0) {
    uint32_t filesize;
    std::memcpy(&filesize, src + 4, sizeof(filesize));
    filesize = athena::utility::LittleUint32(filesize);
    currentOffset += 4;
  }

  auto uncompressedData = std::unique_ptr<uint8_t[]>(new uint8_t[uncompressedLen]);
  uint8_t* outputPtr = uncompressedData.get();
  uint8_t* outputEndPtr = uncompressedData.get() + uncompressedLen;
  const uint8_t* inputPtr = src + currentOffset;
  const uint8_t* inputEndPtr = src + srcLength;

  LZLengthOffset decoding;
  const uint8_t maxTwoByteMatch = 0xF + 1;
  const uint8_t threeByteDenorm = maxTwoByteMatch + 1; // Amount to add to length when compression is 3 bytes
  const uint16_t maxThreeByteMatch = 0xFF + threeByteDenorm;
  const uint16_t fourByteDenorm = maxThreeByteMatch + 1;

  while (inputPtr < inputEndPtr && outputPtr < outputEndPtr) {
    const uint8_t isCompressed = *inputPtr++;

    for (int32_t i = 0; i < m_blockSize; i++) {
      // Checks to see if the next byte is compressed by looking
      // at its binary representation - E.g 10010000
      // This says that the first extracted byte and the four extracted byte is compressed
      if ((isCompressed >> (7 - i)) & 0x1) {
        const uint8_t metaDataSize = *inputPtr >> 4; // Look at the top 4 bits

        if (metaDataSize >= 2) { // Two Bytes of Length/Offset MetaData
          uint16_t lenOff = 0;
          memcpy(&lenOff, inputPtr, 2);
          inputPtr += 2;
          athena::utility::BigUint16(lenOff);
          decoding.length = (lenOff >> 12) + 1;
          decoding.offset = (lenOff & 0xFFF) + 1;
        } else if (metaDataSize == 0) { // Three Bytes of Length/Offset MetaData
          uint32_t lenOff = 0;
          memcpy(reinterpret_cast<uint8_t*>(&lenOff) + 1, inputPtr, 3);
          inputPtr += 3;
          athena::utility::BigUint32(lenOff);
          decoding.length = (lenOff >> 12) + threeByteDenorm;
          decoding.offset = (lenOff & 0xFFF) + 1;
        } else if (metaDataSize == 1) { // Four Bytes of Length/Offset MetaData
          uint32_t lenOff = 0;
          memcpy(&lenOff, inputPtr, 4);
          inputPtr += 4;
          athena::utility::BigUint32(lenOff);

          decoding.length = ((lenOff >> 12) & 0xFFFF) + fourByteDenorm; // Gets rid of the Four byte flag
          decoding.offset = (lenOff & 0xFFF) + 1;
        } else {
          return 0;
        }

        // If the offset to look for uncompressed is passed the
        // current uncompresed data then the data is not compressed
        if ((outputPtr - decoding.offset) < uncompressedData.get()) {
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
  return uncompressedLen;
}
