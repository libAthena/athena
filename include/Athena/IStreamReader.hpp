#ifndef ISTREAMREADER_HPP
#define ISTREAMREADER_HPP

#include <locale>
#include <codecvt>
#include "IStream.hpp"

namespace Athena
{
namespace io
{
class IStreamReader : public IStream
{
public:
    virtual ~IStreamReader() {}

    /*! \brief Sets the Endianss of the stream
     *
     *  \param endian The Endianess to set \sa Endian
     */
    inline void setEndian(Endian endian)
    {m_endian = endian;}

    /*! \brief Returns the current Endianness of the stream
     *
     *  \return Endian The current Stream Endianess
     */
    inline Endian endian()      const
    {return m_endian;}

    /*! \brief Returns whether the stream is BigEndian
     *
     *  \return bool True for BigEndian; False for LittleEndian
     */
    inline bool isBigEndian()   const
    {return (m_endian == Endian::BigEndian);}

    /*! \brief Returns whether the stream is LittleEndian
     *
     *  \return bool True for LittleEndian; False for BigEndian
     */
    inline bool isLittleEndian()const
    {return (m_endian == Endian::LittleEndian);}


    /*! \brief Sets the buffers position relative to the specified position.<br />
     *         It seeks relative to the current position by default.
     *  \param position where in the buffer to seek
     *  \param origin The Origin to seek \sa SeekOrigin
     */
    virtual void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current)=0;

    /*! \brief Sets the buffers position relative to the next 32-byte aligned position.<br />
     */
    inline void seekAlign32() {seek(ROUND_UP_32(position()), SeekOrigin::Begin);}

    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    inline bool atEnd() const
    {return position() >= length();}

    /*! \brief Returns the current position in the stream.
     *
     *  \return Int64 The current position in the stream.
     */
    virtual atUint64 position() const=0;

    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    virtual atUint64 length() const=0;

    /*! \brief Reads a byte at the current position and advances the current position
     *
     * \return Int8 The value at the current position
     */
    inline atInt8 readByte() {atInt8 val; readUBytesToBuf(&val, 1); return val;}

    /*! \brief Reads a byte at the current position and advances the current position
     *
     * \return Uint8 The value at the current position
     */
    inline atUint8 readUByte() {return readByte();}

    /*! \brief Reads a byte at the current position and advances the current position.
     *
     * \return Uint8* The buffer at the current position from the given length.
     */
    inline atInt8* readBytes(atUint64 length)
    {atInt8* buf = new atInt8[length]; readUBytesToBuf(buf, length); return buf;}

    /*! \brief Reads a byte at the current position and advances the current position.
     *
     * \return Int8* The buffer at the current position from the given length.
     */
    inline atUint8* readUBytes(atUint64 length) {return (atUint8*)readBytes(length);}

    inline atUint64 readBytesToBuf(void* buf, atUint64 len) {return readUBytesToBuf(buf, len);}
    virtual atUint64 readUBytesToBuf(void* buf, atUint64 len)=0;

    /*! \brief Reads a Int16 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int16 The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atInt16 readInt16()
    {
        atInt16 val;
        readUBytesToBuf(&val, 2);
        return m_endian == BigEndian ? utility::BigInt16(val) : utility::LittleInt16(val);
    }

    /*! \brief Reads a Uint16 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint16 The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atUint16 readUint16()
    {return readInt16();}

    /*! \brief Reads a Int32 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int32 The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atInt32 readInt32()
    {
        atInt32 val;
        readUBytesToBuf(&val, 4);
        return m_endian == BigEndian ? utility::BigInt32(val) : utility::LittleInt32(val);
    }

    /*! \brief Reads a Uint32 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint32 The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atUint32 readUint32()
    {return readInt32();}

    /*! \brief Reads a Int64 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int64 The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atInt64 readInt64()
    {
        atInt64 val;
        readUBytesToBuf(&val, 8);
        return m_endian == BigEndian ? utility::BigInt64(val) : utility::LittleInt64(val);
    }

    /*! \brief Reads a Uint64 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint64 The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atUint64 readUint64()
    {return readInt64();}

    /*! \brief Reads a float and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return float The value at the current address
     *  \throw IOException when address is out of range
     */
    inline float readFloat()
    {
        float val;
        readUBytesToBuf(&val, 4);
        return m_endian == BigEndian ? utility::BigFloat(val) : utility::LittleFloat(val);
    }

    /*! \brief Reads a double and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return double The value at the current address
     *  \throw IOException when address is out of range
     */
    inline double readDouble()
    {
        double val;
        readUBytesToBuf(&val, 8);
        return m_endian == BigEndian ? utility::BigDouble(val) : utility::LittleDouble(val);
    }

    /*! \brief Reads a bool and advances the current position
     *
     *  \return bool The value at the current address
     *  \throw IOException when address is out of range
     */
    inline bool readBool()
    {
        atUint8 val;
        readUBytesToBuf(&val, 1);
        return val != 0;
    }

    /*! \brief Reads an atVec2f (8 bytes) and advances the current position
     *
     *  \return atVec2f The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atVec2f readVec3f()
    {
        atVec2f val;
        readUBytesToBuf(&val, 8);
        if (m_endian == BigEndian)
        {
            utility::BigFloat(val.vec[0]);
            utility::BigFloat(val.vec[1]);
        }
        else
        {
            utility::LittleFloat(val.vec[0]);
            utility::LittleFloat(val.vec[1]);
        }
        return val;
    }

    /*! \brief Reads an atVec3f (12 bytes) and advances the current position
     *
     *  \return atVec3f The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atVec3f readVec3f()
    {
        atVec3f val;
        readUBytesToBuf(&val, 12);
        if (m_endian == BigEndian)
        {
            utility::BigFloat(val.vec[0]);
            utility::BigFloat(val.vec[1]);
            utility::BigFloat(val.vec[2]);
        }
        else
        {
            utility::LittleFloat(val.vec[0]);
            utility::LittleFloat(val.vec[1]);
            utility::LittleFloat(val.vec[2]);
        }
        return val;
    }

    /*! \brief Reads an atVec4f (16 bytes) and advances the current position
     *
     *  \return atVec4f The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atVec4f readVec4f()
    {
        atVec4f val;
        readUBytesToBuf(&val, 16);
        if (m_endian == BigEndian)
        {
            utility::BigFloat(val.vec[0]);
            utility::BigFloat(val.vec[1]);
            utility::BigFloat(val.vec[2]);
            utility::BigFloat(val.vec[3]);
        }
        else
        {
            utility::LittleFloat(val.vec[0]);
            utility::LittleFloat(val.vec[1]);
            utility::LittleFloat(val.vec[2]);
            utility::LittleFloat(val.vec[3]);
        }
        return val;
    }

    /*! \brief Reads a Unicode string and advances the position in the file
     *
     *  \param fixedLen If non-negative, this is a fixed-length string read
     *  \return std::string The value at the current address
     *  \throw IOException when address is out of range
     */
    inline std::string readUnicode(atInt32 fixedLen = -1)
    {
        std::wstring tmp;
        atUint16 chr = readUint16();

        atInt32 i;
        for (i = 0 ;; ++i)
        {
            if (fixedLen >= 0 && i >= fixedLen - 1)
                break;

            if (!chr)
                break;

            tmp.push_back(chr);
            chr = readUint16();
        }

        if (fixedLen >= 0 && i < fixedLen)
            seek(fixedLen - i);

        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        return conv.to_bytes(tmp);
    }

    /*! \brief Reads a string and advances the position in the file
     *
     *  \param fixedLen If non-negative, this is a fixed-length string read
     *  \return std::string The value at the current address
     *  \throw IOException when address is out of range
     */
    inline std::string readString(atInt32 fixedLen = -1)
    {
        std::string ret;
        atUint8 chr = readByte();

        atInt32 i;
        for (i = 1 ; chr != 0 ; ++i)
        {
            ret += chr;

            if (fixedLen >= 0 && i >= fixedLen)
                break;

            chr = readByte();
        }

        if (fixedLen >= 0 && i < fixedLen)
            seek(fixedLen - i);

        return ret;
    }

    /*! \brief Reads a wstring and advances the position in the file
     *
     *  \param fixedLen If non-negative, this is a fixed-length string read
     *  \return std::wstring The value at the current address
     *  \throw IOException when address is out of range
     */
    inline std::wstring readWString(atInt32 fixedLen = -1)
    {
        std::wstring ret;
        atUint16 chr = readUint16();

        atInt32 i;
        for (i = 1 ; chr != 0 ; ++i)
        {
            ret += chr;

            if (fixedLen >= 0 && i >= fixedLen)
                break;

            chr = readUint16();
        }

        if (fixedLen >= 0 && i < fixedLen)
            seek(fixedLen - i);

        return ret;
    }

protected:
    Endian      m_endian;
};
}
}
#endif // ISTREAMREADER

