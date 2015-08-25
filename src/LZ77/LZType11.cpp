#include "LZ77/LZLookupTable.hpp"
#include "LZ77/LZType11.hpp"
#include <Athena/MemoryWriter.hpp>
#include <string.h>


LZType11::LZType11(atInt32 minimumOffset, atInt32 slidingWindow, atInt32 minimumMatch, atInt32 blockSize)
    : LZBase(minimumOffset, slidingWindow, minimumMatch, blockSize)
{
    m_readAheadBuffer = (0xF + 0xFF + 0xFFFF + m_minMatch);
    m_lookupTable.setLookAheadWindow(m_readAheadBuffer);
}

atUint32 LZType11::compress(const atUint8* src, atUint8** dst, atUint32 srcLength)
{
    Athena::io::MemoryCopyWriter<> outbuff("tmp");

    if (srcLength > 0xFFFFFF) // If length is greater than 24 bits or 16 Megs
    {
        atUint32 encodeFlag = 0x11;
        Athena::utility::LittleUint32(encodeFlag);
        Athena::utility::LittleUint32(srcLength);//Filesize data is little endian
        outbuff.writeUint32(encodeFlag);
        outbuff.writeUint32(srcLength);

    }
    else
    {
        atUint32 encodeSize = (srcLength << 8) | (0x11);
        Athena::utility::LittleUint32(encodeSize);
        outbuff.writeUint32(encodeSize);
    }

    atUint8* ptrStart = (atUint8*)src;
    atUint8* ptrEnd = (atUint8*)(src + srcLength);

    //At most their will be two bytes written if the bytes can be compressed. So if all bytes in the block can be compressed it would take blockSize*2 bytes
    atUint8* compressedBytes = new atUint8[m_blockSize * 2]; //Holds the compressed bytes yet to be written

    atUint8 maxTwoByteMatch = 0xF + 1;
    atUint8 minThreeByteMatch = maxTwoByteMatch + 1; //Minimum Three byte match is maximum TwoByte match + 1
    atUint16 maxThreeByteMatch = 0xFF + minThreeByteMatch;
    atUint16 minFourByteMatch = maxThreeByteMatch + 1; //Minimum Four byte match is maximum Three Byte match + 1
    atInt32 maxFourByteMatch = 0xFFFF + minFourByteMatch;

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
    while (ptrStart < ptrEnd)
    {
        atUint8 blockSize = 0;
        //In Binary represents 1 if byte is compressed or 0 if not compressed
        //For example 01001000 means that the second and fifth byte in the blockSize from the left is compressed
        atUint8* ptrBytes = compressedBytes;

        for (atInt32 i = 0; i < m_blockSize; i++)
        {
            //length_offset searchResult=Search(filedata,ptrStart,ptrEnd);
            LZLengthOffset searchResult = m_lookupTable.search(ptrStart, src, ptrEnd);

            //If the number of bytes to be compressed is at least the size of the Minimum match
            if (searchResult.length >= (atUint32)m_minMatch)
            {
                //Gotta swap the bytes since system is wii is big endian and most computers are little endian

                if (searchResult.length <= maxTwoByteMatch)
                {
                    atUint16 lenOff = ((((searchResult.length - 1) & 0xF) << 12) | //Bits 15-12
                                       ((searchResult.offset - 1) & 0xFFF)               //Bits 11-0
                                      );
                    Athena::utility::BigUint16(lenOff);
                    memcpy(ptrBytes, &lenOff, 2);
                    ptrBytes += 2;
                }
                else if (searchResult.length <= maxThreeByteMatch)
                {
                    atUint32 lenOff = ((((searchResult.length - minThreeByteMatch) & 0xFF) << 12) | //Bits 20-12
                                       ((searchResult.offset - 1) & 0xFFF)                    //Bits 11-0
                                      );
                    Athena::utility::BigUint32(lenOff);
                    memcpy(ptrBytes, (atUint8*)&lenOff + 1, 3); //Make sure to copy the lower 24 bits. 0x12345678- This statement copies 0x123456
                    ptrBytes += 3;
                }
                else if (searchResult.length <= (atUint32)maxFourByteMatch)
                {
                    atUint32 lenOff = ((1 << 28) |                       //Bits 31-28 Flag to say that this is four bytes
                                       (((searchResult.length - minFourByteMatch) & 0xFFFF) << 12) | //Bits 28-12
                                       ((searchResult.offset - 1) & 0xFFF)                        //Bits 11-0
                                      );
                    Athena::utility::BigUint32(lenOff);
                    memcpy(ptrBytes, &lenOff, 4);
                    ptrBytes += 4;
                }

                ptrStart += searchResult.length;

                blockSize |= (1 << (7 - i));
                //Stores which of the next 8 bytes is compressed
                //bit 1 for compress and bit 0 for not compressed
            }
            else
                *ptrBytes++ = *ptrStart++;

        }

        outbuff.writeByte(blockSize);
        outbuff.writeUBytes(compressedBytes, (atUint64)(ptrBytes - compressedBytes));

    }

    delete []compressedBytes;
    compressedBytes = NULL;

    //Add zeros until the file is a multiple of 4
    while ((outbuff.position() % 4) != 0)
        outbuff.writeByte(0);

    *dst = outbuff.data();
    return (atUint32)outbuff.length();
}

atUint32 LZType11::decompress(const atUint8* src, atUint8** dst, atUint32 srcLength)
{
    if (*(atUint8*)(src) != 0x11)
        return 0;

    atUint32 uncompressedLen = *(atUint32*)(src);
    Athena::utility::LittleUint32(uncompressedLen);//The compressed file has the filesize encoded in little endian
    uncompressedLen = uncompressedLen >> 8; //First byte is the encode flag
    atUint32 currentOffset = 4;

    if (uncompressedLen == 0) //If the filesize var is zero then the true filesize is over 14MB and must be read in from the next 4 bytes
    {
        atUint32 filesize = *(atUint32*)(src + 4);
        filesize = Athena::utility::LittleUint32(filesize);
        currentOffset += 4;
    }

    atUint8* uncompressedData = new atUint8[uncompressedLen];
    atUint8* outputPtr = uncompressedData;
    atUint8* outputEndPtr = uncompressedData + uncompressedLen;
    atUint8* inputPtr = (atUint8*)src + currentOffset;
    atUint8* inputEndPtr = (atUint8*)src + srcLength;

    LZLengthOffset decoding;
    atUint8 maxTwoByteMatch = 0xF + 1;
    atUint8 threeByteDenorm = maxTwoByteMatch + 1; //Amount to add to length when compression is 3 bytes
    atUint16 maxThreeByteMatch = 0xFF + threeByteDenorm;
    atUint16 fourByteDenorm = maxThreeByteMatch + 1;

    while (inputPtr < inputEndPtr && outputPtr < outputEndPtr)
    {

        atUint8 isCompressed = *inputPtr++;

        for (atInt32 i = 0; i < m_blockSize; i++)
        {
            //Checks to see if the next byte is compressed by looking
            //at its binary representation - E.g 10010000
            //This says that the first extracted byte and the four extracted byte is compressed
            if ((isCompressed >> (7 - i)) & 0x1)
            {
                atUint8 metaDataSize = *inputPtr >> 4; //Look at the top 4 bits

                if (metaDataSize >= 2) //Two Bytes of Length/Offset MetaData
                {
                    atUint16 lenOff = 0;
                    memcpy(&lenOff, inputPtr, 2);
                    inputPtr += 2;
                    Athena::utility::BigUint16(lenOff);
                    decoding.length = (lenOff >> 12) + 1;
                    decoding.offset = (lenOff & 0xFFF) + 1;
                }
                else if (metaDataSize == 0) //Three Bytes of Length/Offset MetaData
                {
                    atUint32 lenOff = 0;
                    memcpy((atUint8*)&lenOff + 1, inputPtr, 3);
                    inputPtr += 3;
                    Athena::utility::BigUint32(lenOff);
                    decoding.length = (lenOff >> 12) + threeByteDenorm;
                    decoding.offset = (lenOff & 0xFFF) + 1;
                }
                else if (metaDataSize == 1) //Four Bytes of Length/Offset MetaData
                {
                    atUint32 lenOff = 0;
                    memcpy(&lenOff, inputPtr, 4);
                    inputPtr += 4;
                    Athena::utility::BigUint32(lenOff);

                    decoding.length = ((lenOff >> 12) & 0xFFFF) + fourByteDenorm; //Gets rid of the Four byte flag
                    decoding.offset = (lenOff & 0xFFF) + 1;
                }
                else
                {
                    delete[] uncompressedData;
                    uncompressedData = nullptr;
                    return 0;
                }

                if ((outputPtr - decoding.offset) < uncompressedData) //If the offset to look for uncompressed is passed the current uncompresed data then the data is not compressed
                {
                    delete []uncompressedData;
                    return 0;
                }

                for (atUint32 j = 0; j < decoding.length; ++j)
                    outputPtr[j] = (outputPtr - decoding.offset)[j];

                outputPtr += decoding.length;
            }
            else
                *outputPtr++ = *inputPtr++;

            if (!(inputPtr < inputEndPtr && outputPtr < outputEndPtr))
                break;


        }
    }

    *dst = uncompressedData;
    return uncompressedLen;
}

