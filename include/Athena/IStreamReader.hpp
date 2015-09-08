#ifndef ISTREAMREADER_HPP
#define ISTREAMREADER_HPP

#include <memory>
#include <functional>
#include "utf8proc.h"
#include "IStream.hpp"

namespace Athena
{
namespace io
{
class IStreamReader : public IStream
{
public:
    virtual ~IStreamReader() {}

    /** @brief Sets the Endianness of the stream
     *
     *  @param endian The Endianness to set
     */
    inline void setEndian(Endian endian)
    {m_endian = endian;}

    /** @brief Returns the current Endianness of the stream
     *
     *  @return The current Stream Endianness
     */
    inline Endian endian()      const
    {return m_endian;}

    /** @brief Returns whether the stream is BigEndian
     *
     *  @return bool True for BigEndian; False for LittleEndian
     */
    inline bool isBigEndian()   const
    {return (m_endian == Endian::BigEndian);}

    /** @brief Returns whether the stream is LittleEndian
     *
     *  @return True for LittleEndian; False for BigEndian
     */
    inline bool isLittleEndian()const
    {return (m_endian == Endian::LittleEndian);}


    /** @brief Sets the buffers position relative to the specified position.<br />
     *         It seeks relative to the current position by default.
     *  @param position where in the buffer to seek
     *  @param origin The Origin to seek relative to
     */
    virtual void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current)=0;

    /** @brief Sets the buffers position relative to the next 32-byte aligned position.<br />
     */
    inline void seekAlign32() {seek(ROUND_UP_32(position()), SeekOrigin::Begin);}

    /** @brief Returns whether or not the stream is at the end.
     *
     *  @return True if at end; False otherwise.
     */
    inline bool atEnd() const
    {return position() >= length();}

    /** @brief Returns the current position in the stream.
     *
     *  @return The current position in the stream.
     */
    virtual atUint64 position() const=0;

    /** @brief Returns whether or not the stream is at the end.
     *
     *  @return True if at end; False otherwise.
     */
    virtual atUint64 length() const=0;

    /** @brief Reads a byte at the current position and advances the current position
     *
     * @return The value at the current position
     */
    inline atInt8 readByte() {atInt8 val; readUBytesToBuf(&val, 1); return val;}
    template <class T>
    inline atInt8 readVal(typename std::enable_if<std::is_same<T, atInt8>::value>::type* = 0)
    {return readByte();}

    /** @brief Reads a byte at the current position and advances the current position
     *
     * @return The value at the current position
     */
    inline atUint8 readUByte() {return readByte();}
    template <class T>
    inline atUint8 readVal(typename std::enable_if<std::is_same<T, atUint8>::value>::type* = 0)
    {return readUByte();}

    /** @brief Reads a byte at the current position and advances the current position.
     *
     * @return The buffer at the current position from the given length.
     */
    inline std::unique_ptr<atInt8[]> readBytes(atUint64 length)
    {
        atInt8* buf = new atInt8[length];
        readUBytesToBuf(buf, length);
        return std::unique_ptr<atInt8[]>(buf);
    }

    /** @brief Reads a byte at the current position and advances the current position.
     *
     * @return The buffer at the current position from the given length.
     */
    inline std::unique_ptr<atUint8[]> readUBytes(atUint64 length)
    {
        atUint8* buf = new atUint8[length];
        readUBytesToBuf(buf, length);
        return std::unique_ptr<atUint8[]>(buf);
    }

    inline atUint64 readBytesToBuf(void* buf, atUint64 len) {return readUBytesToBuf(buf, len);}
    virtual atUint64 readUBytesToBuf(void* buf, atUint64 len)=0;

    /** @brief Reads a Int16 and swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atInt16 readInt16()
    {
        atInt16 val;
        readUBytesToBuf(&val, 2);
        return m_endian == BigEndian ? utility::BigInt16(val) : utility::LittleInt16(val);
    }
    template <class T>
    inline atInt16 readVal(typename std::enable_if<std::is_same<T, atInt16>::value>::type* = 0)
    {return readInt16();}

    /** @brief Reads a Int16 and swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atInt16 readInt16Little()
    {
        atInt16 val;
        readUBytesToBuf(&val, 2);
        return utility::LittleInt16(val);
    }
    template <class T>
    inline atInt16 readValLittle(typename std::enable_if<std::is_same<T, atInt16>::value>::type* = 0)
    {return readInt16Little();}

    /** @brief Reads a Int16 and swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atInt16 readInt16Big()
    {
        atInt16 val;
        readUBytesToBuf(&val, 2);
        return utility::BigInt16(val);
    }
    template <class T>
    inline atInt16 readValBig(typename std::enable_if<std::is_same<T, atInt16>::value>::type* = 0)
    {return readInt16Big();}

    /** @brief Reads a Uint16 and swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atUint16 readUint16()
    {return readInt16();}
    template <class T>
    inline atUint16 readVal(typename std::enable_if<std::is_same<T, atUint16>::value>::type* = 0)
    {return readUint16();}

    /** @brief Reads a Uint16 and swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atUint16 readUint16Little()
    {
        atUint16 val;
        readUBytesToBuf(&val, 2);
        return utility::LittleUint16(val);
    }
    template <class T>
    inline atUint16 readValLittle(typename std::enable_if<std::is_same<T, atUint16>::value>::type* = 0)
    {return readUint16Little();}

    /** @brief Reads a Uint16 and swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atUint16 readUint16Big()
    {
        atUint16 val;
        readUBytesToBuf(&val, 2);
        return utility::BigUint16(val);
    }
    template <class T>
    inline atUint16 readValBig(typename std::enable_if<std::is_same<T, atUint16>::value>::type* = 0)
    {return readUint16Big();}

    /** @brief Reads a Int32 and swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atInt32 readInt32()
    {
        atInt32 val;
        readUBytesToBuf(&val, 4);
        return m_endian == BigEndian ? utility::BigInt32(val) : utility::LittleInt32(val);
    }
    template <class T>
    inline atInt32 readVal(typename std::enable_if<std::is_same<T, atInt32>::value>::type* = 0)
    {return readInt32();}

    /** @brief Reads a Int32 and swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atInt32 readInt32Little()
    {
        atInt32 val;
        readUBytesToBuf(&val, 4);
        return utility::LittleInt32(val);
    }
    template <class T>
    inline atInt32 readValLittle(typename std::enable_if<std::is_same<T, atInt32>::value>::type* = 0)
    {return readInt32Little();}

    /** @brief Reads a Int32 and swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atInt32 readInt32Big()
    {
        atInt32 val;
        readUBytesToBuf(&val, 4);
        return utility::BigInt32(val);
    }
    template <class T>
    inline atInt32 readValBig(typename std::enable_if<std::is_same<T, atInt32>::value>::type* = 0)
    {return readInt32Big();}

    /** @brief Reads a Uint32 and swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atUint32 readUint32()
    {return readInt32();}
    template <class T>
    inline atUint32 readVal(typename std::enable_if<std::is_same<T, atUint32>::value>::type* = 0)
    {return readUint32();}

    /** @brief Reads a Uint32 and swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atUint32 readUint32Little()
    {
        atUint32 val;
        readUBytesToBuf(&val, 4);
        return utility::LittleUint32(val);
    }
    template <class T>
    inline atInt32 readValLittle(typename std::enable_if<std::is_same<T, atUint32>::value>::type* = 0)
    {return readUint32Little();}

    /** @brief Reads a Uint32 and swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atUint32 readUint32Big()
    {
        atUint32 val;
        readUBytesToBuf(&val, 4);
        return utility::BigUint32(val);
    }
    template <class T>
    inline atUint32 readValBig(typename std::enable_if<std::is_same<T, atUint32>::value>::type* = 0)
    {return readUint32Big();}

    /** @brief Reads a Int64 and swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atInt64 readInt64()
    {
        atInt64 val;
        readUBytesToBuf(&val, 8);
        return m_endian == BigEndian ? utility::BigInt64(val) : utility::LittleInt64(val);
    }
    template <class T>
    inline atInt64 readVal(typename std::enable_if<std::is_same<T, atInt64>::value>::type* = 0)
    {return readInt64();}

    /** @brief Reads a Int64 and swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atInt64 readInt64Little()
    {
        atInt64 val;
        readUBytesToBuf(&val, 8);
        return utility::LittleInt64(val);
    }
    template <class T>
    inline atInt64 readValLittle(typename std::enable_if<std::is_same<T, atInt64>::value>::type* = 0)
    {return readInt64Little();}

    /** @brief Reads a Int64 and swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atInt64 readInt64Big()
    {
        atInt64 val;
        readUBytesToBuf(&val, 8);
        return utility::BigInt64(val);
    }
    template <class T>
    inline atInt64 readValBig(typename std::enable_if<std::is_same<T, atInt64>::value>::type* = 0)
    {return readInt64Big();}

    /** @brief Reads a Uint64 and swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atUint64 readUint64()
    {return readInt64();}
    template <class T>
    inline atUint64 readVal(typename std::enable_if<std::is_same<T, atUint64>::value>::type* = 0)
    {return readUint64();}

    /** @brief Reads a Uint64 and swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atUint64 readUint64Little()
    {
        atUint64 val;
        readUBytesToBuf(&val, 8);
        return utility::LittleUint64(val);
    }
    template <class T>
    inline atUint64 readValLittle(typename std::enable_if<std::is_same<T, atUint64>::value>::type* = 0)
    {return readUint64Little();}

    /** @brief Reads a Uint64 and swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atUint64 readUint64Big()
    {
        atUint64 val;
        readUBytesToBuf(&val, 8);
        return utility::BigUint64(val);
    }
    template <class T>
    inline atUint64 readValBig(typename std::enable_if<std::is_same<T, atUint64>::value>::type* = 0)
    {return readUint64Big();}

    /** @brief Reads a float and swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline float readFloat()
    {
        float val;
        readUBytesToBuf(&val, 4);
        return m_endian == BigEndian ? utility::BigFloat(val) : utility::LittleFloat(val);
    }
    template <class T>
    inline float readVal(typename std::enable_if<std::is_same<T, float>::value>::type* = 0)
    {return readFloat();}

    /** @brief Reads a float and swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline float readFloatLittle()
    {
        float val;
        readUBytesToBuf(&val, 4);
        return utility::LittleFloat(val);
    }
    template <class T>
    inline float readValLittle(typename std::enable_if<std::is_same<T, float>::value>::type* = 0)
    {return readFloatLittle();}

    /** @brief Reads a float and swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline float readFloatBig()
    {
        float val;
        readUBytesToBuf(&val, 4);
        return utility::BigFloat(val);
    }
    template <class T>
    inline float readValBig(typename std::enable_if<std::is_same<T, float>::value>::type* = 0)
    {return readFloatBig();}

    /** @brief Reads a double and swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline double readDouble()
    {
        double val;
        readUBytesToBuf(&val, 8);
        return m_endian == BigEndian ? utility::BigDouble(val) : utility::LittleDouble(val);
    }
    template <class T>
    inline double readVal(typename std::enable_if<std::is_same<T, double>::value>::type* = 0)
    {return readDouble();}

    /** @brief Reads a double and swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline double readDoubleLittle()
    {
        double val;
        readUBytesToBuf(&val, 8);
        return utility::LittleDouble(val);
    }
    template <class T>
    inline double readValLittle(typename std::enable_if<std::is_same<T, double>::value>::type* = 0)
    {return readDoubleLittle();}

    /** @brief Reads a double and swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline double readDoubleBig()
    {
        double val;
        readUBytesToBuf(&val, 8);
        return utility::BigDouble(val);
    }
    template <class T>
    inline double readValBig(typename std::enable_if<std::is_same<T, double>::value>::type* = 0)
    {return readDoubleBig();}

    /** @brief Reads a bool and advances the current position
     *
     *  @return The value at the current address
     */
    inline bool readBool()
    {
        atUint8 val;
        readUBytesToBuf(&val, 1);
        return val != 0;
    }
    template <class T>
    inline bool readVal(typename std::enable_if<std::is_same<T, bool>::value>::type* = 0)
    {return readBool();}

    /** @brief Reads an atVec2f (8 bytes), swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atVec2f readVec2f()
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
    template <class T>
    inline atVec2f readVal(typename std::enable_if<std::is_same<T, atVec2f>::value>::type* = 0)
    {return readVec2f();}

    /** @brief Reads an atVec2f (8 bytes), swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atVec2f readVec2fLittle()
    {
        atVec2f val;
        readUBytesToBuf(&val, 8);
        utility::LittleFloat(val.vec[0]);
        utility::LittleFloat(val.vec[1]);
        return val;
    }
    template <class T>
    inline atVec2f readValLittle(typename std::enable_if<std::is_same<T, atVec2f>::value>::type* = 0)
    {return readVec2fLittle();}

    /** @brief Reads an atVec2f (8 bytes), swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atVec2f readVec2fBig()
    {
        atVec2f val;
        readUBytesToBuf(&val, 8);
        utility::BigFloat(val.vec[0]);
        utility::BigFloat(val.vec[1]);
        return val;
    }
    template <class T>
    inline atVec2f readValBig(typename std::enable_if<std::is_same<T, atVec2f>::value>::type* = 0)
    {return readVec2fBig();}

    /** @brief Reads an atVec3f (12 bytes), swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
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
    template <class T>
    inline atVec3f readVal(typename std::enable_if<std::is_same<T, atVec3f>::value>::type* = 0)
    {return readVec3f();}

    /** @brief Reads an atVec3f (12 bytes), swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atVec3f readVec3fLittle()
    {
        atVec3f val;
        readUBytesToBuf(&val, 12);
        utility::LittleFloat(val.vec[0]);
        utility::LittleFloat(val.vec[1]);
        utility::LittleFloat(val.vec[2]);
        return val;
    }
    template <class T>
    inline atVec3f readValLittle(typename std::enable_if<std::is_same<T, atVec3f>::value>::type* = 0)
    {return readVec3fLittle();}

    /** @brief Reads an atVec3f (12 bytes), swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atVec3f readVec3fBig()
    {
        atVec3f val;
        readUBytesToBuf(&val, 12);
        utility::BigFloat(val.vec[0]);
        utility::BigFloat(val.vec[1]);
        utility::BigFloat(val.vec[2]);
        return val;
    }
    template <class T>
    inline atVec3f readValBig(typename std::enable_if<std::is_same<T, atVec3f>::value>::type* = 0)
    {return readVec3fBig();}

    /** @brief Reads an atVec4f (16 bytes), swaps to endianness specified by setEndian depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
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
    template <class T>
    inline atVec4f readVal(typename std::enable_if<std::is_same<T, atVec4f>::value>::type* = 0)
    {return readVec4f();}

    /** @brief Reads an atVec4f (16 bytes), swaps against little endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atVec4f readVec4fLittle()
    {
        atVec4f val;
        readUBytesToBuf(&val, 16);
        utility::LittleFloat(val.vec[0]);
        utility::LittleFloat(val.vec[1]);
        utility::LittleFloat(val.vec[2]);
        utility::LittleFloat(val.vec[3]);
        return val;
    }
    template <class T>
    inline atVec4f readValLittle(typename std::enable_if<std::is_same<T, atVec4f>::value>::type* = 0)
    {return readVec4fLittle();}

    /** @brief Reads an atVec4f (16 bytes), swaps against big endianness depending on platform
     *  and advances the current position
     *
     *  @return The value at the current address
     */
    inline atVec4f readVec4fBig()
    {
        atVec4f val;
        readUBytesToBuf(&val, 16);
        utility::BigFloat(val.vec[0]);
        utility::BigFloat(val.vec[1]);
        utility::BigFloat(val.vec[2]);
        utility::BigFloat(val.vec[3]);
        return val;
    }
    template <class T>
    inline atVec4f readValBig(typename std::enable_if<std::is_same<T, atVec4f>::value>::type* = 0)
    {return readVec4fBig();}

    /** @brief Reads a wide-char string (using endianness from setEndian),
     *         converts to UTF8 and advances the position in the file
     *
     *  @param fixedLen If non-negative, this is a fixed-length string read
     *  @return The read string
     */
    inline std::string readWStringAsString(atInt32 fixedLen = -1)
    {
        std::string retval;
        atUint16 chr = readUint16();

        atInt32 i;
        for (i=0 ;; ++i)
        {
            if (fixedLen >= 0 && i >= fixedLen - 1)
                break;

            if (!chr)
                break;

            utf8proc_uint8_t mb[4];
            utf8proc_ssize_t c = utf8proc_encode_char(utf8proc_int32_t(chr), mb);
            if (c < 0)
            {
                atWarning("invalid UTF-8 character while encoding");
                return retval;
            }

            retval.append(reinterpret_cast<char*>(mb), c);
            chr = readUint16();
        }

        if (fixedLen >= 0 && i < fixedLen)
            seek(fixedLen - i);

        return retval;
    }

    /** @brief Reads a wide-char string (against little-endian),
     *         converts to UTF8 and advances the position in the file
     *
     *  @param fixedLen If non-negative, this is a fixed-length string read
     *  @return The read string
     */
    inline std::string readWStringAsStringLittle(atInt32 fixedLen = -1)
    {
        std::string retval;
        atUint16 chr = readUint16Little();

        atInt32 i;
        for (i=0 ;; ++i)
        {
            if (fixedLen >= 0 && i >= fixedLen - 1)
                break;

            if (!chr)
                break;

            utf8proc_uint8_t mb[4];
            utf8proc_ssize_t c = utf8proc_encode_char(utf8proc_int32_t(chr), mb);
            if (c < 0)
            {
                atWarning("invalid UTF-8 character while encoding");
                return retval;
            }

            retval.append(reinterpret_cast<char*>(mb), c);
            chr = readUint16Little();
        }

        if (fixedLen >= 0 && i < fixedLen)
            seek(fixedLen - i);

        return retval;
    }

    /** @brief Reads a wide-char string (against big-endian),
     *         converts to UTF8 and advances the position in the file
     *
     *  @param fixedLen If non-negative, this is a fixed-length string read
     *  @return The read string
     */
    inline std::string readWStringAsStringBig(atInt32 fixedLen = -1)
    {
        std::string retval;
        atUint16 chr = readUint16Big();

        atInt32 i;
        for (i = 0 ;; ++i)
        {
            if (fixedLen >= 0 && i >= fixedLen - 1)
                break;

            if (!chr)
                break;

            utf8proc_uint8_t mb[4];
            utf8proc_ssize_t c = utf8proc_encode_char(utf8proc_int32_t(chr), mb);
            if (c < 0)
            {
                atWarning("invalid UTF-8 character while encoding");
                return retval;
            }

            retval.append(reinterpret_cast<char*>(mb), c);
            chr = readUint16Big();
        }

        if (fixedLen >= 0 && i < fixedLen)
            seek(fixedLen - i);

        return retval;
    }

    /** @brief Reads a string and advances the position in the file
     *
     *  @param fixedLen If non-negative, this is a fixed-length string read
     *  @return The read string
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
    template <class T>
    inline std::string readVal(typename std::enable_if<std::is_same<T, std::string>::value>::type* = 0)
    {return readString();}

    /** @brief Reads a wstring and advances the position in the file
     *
     *  @param fixedLen If non-negative, this is a fixed-length string read
     *  @return The read wstring
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
    template <class T>
    inline std::wstring readVal(typename std::enable_if<std::is_same<T, std::wstring>::value>::type* = 0)
    {return readWString();}

    /** @brief Reads a wstring assuming little-endian characters
     *         and advances the position in the file
     *
     *  @param fixedLen If non-negative, this is a fixed-length string read
     *  @return The read wstring
     */
    inline std::wstring readWStringLittle(atInt32 fixedLen = -1)
    {
        std::wstring ret;
        atUint16 chr = readUint16Little();

        atInt32 i;
        for (i = 1 ; chr != 0 ; ++i)
        {
            ret += chr;

            if (fixedLen >= 0 && i >= fixedLen)
                break;

            chr = readUint16Little();
        }

        if (fixedLen >= 0 && i < fixedLen)
            seek(fixedLen - i);

        return ret;
    }
    template <class T>
    inline std::wstring readValLittle(typename std::enable_if<std::is_same<T, std::wstring>::value>::type* = 0)
    {return readWStringLittle();}

    /** @brief Reads a wstring assuming big-endian characters
     *         and advances the position in the file
     *
     *  @param fixedLen If non-negative, this is a fixed-length string read
     *  @return The read wstring
     */
    inline std::wstring readWStringBig(atInt32 fixedLen = -1)
    {
        std::wstring ret;
        atUint16 chr = readUint16Big();

        atInt32 i;
        for (i = 1 ; chr != 0 ; ++i)
        {
            ret += chr;

            if (fixedLen >= 0 && i >= fixedLen)
                break;

            chr = readUint16Big();
        }

        if (fixedLen >= 0 && i < fixedLen)
            seek(fixedLen - i);

        return ret;
    }
    template <class T>
    inline std::wstring readValBig(typename std::enable_if<std::is_same<T, std::wstring>::value>::type* = 0)
    {return readWStringBig();}

    /** @brief Performs automatic std::vector enumeration reads using numeric type T
     *
     *  @param vector The std::vector to clear and populate using read data
     *  @param count The number of elements to read into vector
     *
     *  Endianness is set with setEndian
     */
    template<class T>
    void enumerate(std::vector<T>& vector, size_t count,
                   typename std::enable_if<std::is_arithmetic<T>::value ||
                                           std::is_same<T, atVec2f>::value ||
                                           std::is_same<T, atVec3f>::value ||
                                           std::is_same<T, atVec4f>::value>::type* = 0)
    {
        vector.clear();
        vector.reserve(count);
        for (size_t i=0 ; i<count ; ++i)
            vector.emplace_back(readVal<T>());
    }

    /** @brief Performs automatic std::vector enumeration reads using numeric type T
     *
     *  @param vector The std::vector to clear and populate using read data
     *  @param count The number of elements to read into vector
     *
     *  Endianness is little
     */
    template<class T>
    void enumerateLittle(std::vector<T>& vector, size_t count,
                         typename std::enable_if<std::is_arithmetic<T>::value ||
                                                 std::is_same<T, atVec2f>::value ||
                                                 std::is_same<T, atVec3f>::value ||
                                                 std::is_same<T, atVec4f>::value>::type* = 0)
    {
        vector.clear();
        vector.reserve(count);
        for (size_t i=0 ; i<count ; ++i)
            vector.emplace_back(readValLittle<T>());
    }

    /** @brief Performs automatic std::vector enumeration reads using numeric type T
     *
     *  @param vector The std::vector to clear and populate using read data
     *  @param count The number of elements to read into vector
     *
     *  Endianness is big
     */
    template<class T>
    void enumerateBig(std::vector<T>& vector, size_t count,
                      typename std::enable_if<std::is_arithmetic<T>::value ||
                                              std::is_same<T, atVec2f>::value ||
                                              std::is_same<T, atVec3f>::value ||
                                              std::is_same<T, atVec4f>::value>::type* = 0)
    {
        vector.clear();
        vector.reserve(count);
        for (size_t i=0 ; i<count ; ++i)
            vector.emplace_back(readValBig<T>());
    }

    /** @brief Performs automatic std::vector enumeration reads using non-numeric type T
     *
     *  @param vector The std::vector to clear and populate using read data
     *  @param count The number of elements to read into vector
     */
    template<class T>
    void enumerate(std::vector<T>& vector, size_t count,
                   typename std::enable_if<!std::is_arithmetic<T>::value &&
                                           !std::is_same<T, atVec2f>::value &&
                                           !std::is_same<T, atVec3f>::value &&
                                           !std::is_same<T, atVec4f>::value>::type* = 0)
    {
        vector.clear();
        vector.reserve(count);
        for (size_t i=0 ; i<count ; ++i)
        {
            vector.emplace_back();
            vector.back().read(*this);
        }
    }

    /** @brief Performs lambda-assisted std::vector enumeration reads using type T
     *
     *  @param vector The std::vector to clear and populate using read data
     *  @param count The number of elements to read into vector
     *  @param readf Function (e.g. a lambda) that reads *one* element and
     *               assigns the value through the second argument
     */
    template<class T>
    void enumerate(std::vector<T>& vector, size_t count, std::function<void(IStreamReader&, T&)> readf)
    {
        vector.clear();
        vector.reserve(count);
        for (size_t i=0 ; i<count ; ++i)
        {
            vector.emplace_back();
            readf(*this, vector.back());
        }
    }

protected:
    Endian      m_endian;
};
}
}
#endif // ISTREAMREADER

