#ifndef ISTREAMWRITER_HPP
#define ISTREAMWRITER_HPP

#include "utf8proc.h"
#include "IStream.hpp"

namespace Athena
{
namespace io
{
class IStreamWriter : public IStream
{
public:
    virtual ~IStreamWriter() {}
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
    inline Endian endian() const
    {return m_endian;}

    /** @brief Returns whether the stream is BigEndian
     *
     *  @return True for BigEndian; False for LittleEndian
     */
    inline bool isBigEndian() const
    {return (m_endian == Endian::BigEndian);}

    /** @brief Returns whether the stream is LittleEndian
     *
     *  @return True for LittleEndian; False for BigEndian
     */
    inline bool isLittleEndian() const
    {return (m_endian == Endian::LittleEndian);}

    /** @brief Sets the buffers position relative to the specified position.<br />
     *         It seeks relative to the current position by default.
     *  @param position where in the buffer to seek
     *  @param origin The location to seek relative to
     */
    virtual void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current)=0;

    /** @brief Sets the buffers position relative to the next 32-byte aligned position.<br />
     */
    inline void seekAlign32() {seek(ROUND_UP_32(position()), SeekOrigin::Begin);}

    /** @brief Returns whether or not the stream is at the end.
     *
     *  @return True if at end; False otherwise.
     */
    inline bool atEnd() const {return position() >= length();}

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

    /** @brief Writes a byte at the current position and advances the position by one byte.
     *  @param val The value to write
     */
    inline void writeUByte(atUint8 val) {writeUBytes(&val, 1);}
    inline void writeVal(atUint8 val) {writeUByte(val);}
    inline void writeValLittle(atUint8 val) {writeUByte(val);}
    inline void writeValBig(atUint8 val) {writeUByte(val);}

    /** @brief Writes a byte at the current position and advances the position by one byte.
     *  @param val The value to write
     */
    inline void writeByte(atInt8 val) {writeUByte(val);}
    inline void writeVal(atInt8 val) {writeByte(val);}
    inline void writeValLittle(atInt8 val) {writeByte(val);}
    inline void writeValBig(atInt8 val) {writeByte(val);}

    /** @brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     *  @param data The buffer to write
     *  @param length The amount to write
     */
    virtual void writeUBytes(const atUint8* data, atUint64 len)=0;

    /** @brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     *  @param data The buffer to write
     *  @param length The amount to write
     */
    inline void writeBytes(const atInt8* data, atUint64 len) {writeUBytes((atUint8*)data, len);}

    /** @brief Writes an Int16 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeInt16(atInt16 val)
    {
        if (m_endian == BigEndian)
            utility::BigInt16(val);
        else
            utility::LittleInt16(val);
        writeUBytes((atUint8*)&val, 2);
    }
    inline void writeVal(atInt16 val) {writeInt16(val);}

    /** @brief Writes an Int16 to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeInt16Little(atInt16 val)
    {
        utility::LittleInt16(val);
        writeUBytes((atUint8*)&val, 2);
    }
    inline void writeValLittle(atInt16 val) {writeInt16Little(val);}

    /** @brief Writes an Int16 to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeInt16Big(atInt16 val)
    {
        utility::BigInt16(val);
        writeUBytes((atUint8*)&val, 2);
    }
    inline void writeValBig(atInt16 val) {writeInt16Big(val);}

    /** @brief Writes an Uint16 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings
     *
     *  @param val The value to write to the buffer
     */
    inline void writeUint16(atUint16 val) {writeInt16(val);}
    inline void writeVal(atUint16 val) {writeUint16(val);}

    /** @brief Writes an Uint16 to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform
     *
     *  @param val The value to write to the buffer
     */
    inline void writeUint16Little(atUint16 val) {writeInt16Little(val);}
    inline void writeValLittle(atUint16 val) {writeUint16Little(val);}

    /** @brief Writes an Uint16 to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform
     *
     *  @param val The value to write to the buffer
     */
    inline void writeUint16Big(atUint16 val) {writeInt16Big(val);}
    inline void writeValBig(atUint16 val) {writeUint16Big(val);}

    /** @brief Writes an Int32 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeInt32(atInt32 val)
    {
        if (m_endian == BigEndian)
            utility::BigInt32(val);
        else
            utility::LittleInt32(val);
        writeUBytes((atUint8*)&val, 4);
    }
    inline void writeVal(atInt32 val) {writeInt32(val);}

    /** @brief Writes an Int32 to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeInt32Little(atInt32 val)
    {
        utility::LittleInt32(val);
        writeUBytes((atUint8*)&val, 4);
    }
    inline void writeValLittle(atInt32 val) {writeInt32Little(val);}

    /** @brief Writes an Int32 to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeInt32Big(atInt32 val)
    {
        utility::BigInt32(val);
        writeUBytes((atUint8*)&val, 4);
    }
    inline void writeValBig(atInt32 val) {writeInt32Big(val);}

    /** @brief Writes an Uint32 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeUint32(atUint32 val) {writeInt32(val);}
    inline void writeVal(atUint32 val) {writeUint32(val);}

    /** @brief Writes an Uint32 to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeUint32Little(atUint32 val) {writeInt32Little(val);}
    inline void writeValLittle(atUint32 val) {writeUint32Little(val);}

    /** @brief Writes an Uint32 to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeUint32Big(atUint32 val) {writeInt32Big(val);}
    inline void writeValBig(atUint32 val) {writeUint32Big(val);}

    /** @brief Writes an Int64 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeInt64(atInt64 val)
    {
        if (m_endian == BigEndian)
            utility::BigInt64(val);
        else
            utility::LittleInt64(val);
        writeUBytes((atUint8*)&val, 8);
    }
    inline void writeVal(atInt64 val) {writeInt64(val);}

    /** @brief Writes an Int64 to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeInt64Little(atInt64 val)
    {
        utility::LittleInt64(val);
        writeUBytes((atUint8*)&val, 8);
    }
    inline void writeValLittle(atInt64 val) {writeInt64Little(val);}

    /** @brief Writes an Int64 to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeInt64Big(atInt64 val)
    {
        utility::BigInt64(val);
        writeUBytes((atUint8*)&val, 8);
    }
    inline void writeValBig(atInt64 val) {writeInt64Big(val);}

    /** @brief Writes an Uint64 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeUint64(atUint64 val) {writeInt64(val);}
    inline void writeVal(atUint64 val) {writeUint64(val);}

    /** @brief Writes an Uint64 to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeUint64Little(atUint64 val) {writeInt64Little(val);}
    inline void writeValLittle(atUint64 val) {writeUint64Little(val);}

    /** @brief Writes an Uint64 to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeUint64Big(atUint64 val) {writeInt64Big(val);}
    inline void writeValBig(atUint64 val) {writeUint64Big(val);}

    /** @brief Writes an float to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeFloat(float val)
    {
        if (m_endian == BigEndian)
            utility::BigFloat(val);
        else
            utility::LittleFloat(val);
        writeUBytes((atUint8*)&val, 4);
    }
    inline void writeVal(float val) {writeFloat(val);}

    /** @brief Writes an float to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeFloatLittle(float val)
    {
        utility::LittleFloat(val);
        writeUBytes((atUint8*)&val, 4);
    }
    inline void writeValLittle(float val) {writeFloatLittle(val);}

    /** @brief Writes an float to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeFloatBig(float val)
    {
        utility::BigFloat(val);
        writeUBytes((atUint8*)&val, 4);
    }
    inline void writeValBig(float val) {writeFloatBig(val);}

    /** @brief Writes an double to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeDouble(double val)
    {
        if (m_endian == BigEndian)
            utility::BigDouble(val);
        else
            utility::LittleDouble(val);
        writeUBytes((atUint8*)&val, 8);
    }
    inline void writeVal(double val) {writeDouble(val);}

    /** @brief Writes an double to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeDoubleLittle(double val)
    {
        utility::LittleDouble(val);
        writeUBytes((atUint8*)&val, 8);
    }
    inline void writeValLittle(double val) {writeDoubleLittle(val);}

    /** @brief Writes an double to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeDoubleBig(double val)
    {
        utility::BigDouble(val);
        writeUBytes((atUint8*)&val, 8);
    }
    inline void writeValBig(double val) {writeDoubleBig(val);}

    /** @brief Writes an bool to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param val The value to write to the buffer
     */
    inline void writeBool(bool val) {writeUBytes((atUint8*)&val, 1);}
    inline void writeVal(bool val) {writeBool(val);}
    inline void writeValLittle(bool val) {writeBool(val);}
    inline void writeValBig(bool val) {writeBool(val);}

    /** @brief Writes an atVec2f (8 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     * @param vec The value to write to the buffer
     */
    inline void writeVec2f(const atVec2f& vec)
    {
        atVec2f tmp = vec;
        if (m_endian == BigEndian)
        {
            utility::BigFloat(tmp.vec[0]);
            utility::BigFloat(tmp.vec[1]);
        }
        else
        {
            utility::LittleFloat(tmp.vec[0]);
            utility::LittleFloat(tmp.vec[1]);
        }
        writeUBytes((atUint8*)&tmp, 8);
    }
    inline void writeVal(const atVec2f& val) {writeVec2f(val);}

    /** @brief Writes an atVec2f (8 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     * @param vec The value to write to the buffer
     */
    inline void writeVec2fLittle(const atVec2f& vec)
    {
        atVec2f tmp = vec;
        utility::LittleFloat(tmp.vec[0]);
        utility::LittleFloat(tmp.vec[1]);
        writeUBytes((atUint8*)&tmp, 8);
    }
    inline void writeValLittle(const atVec2f& val) {writeVec2fLittle(val);}

    /** @brief Writes an atVec2f (8 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     * @param vec The value to write to the buffer
     */
    inline void writeVec2fBig(const atVec2f& vec)
    {
        atVec2f tmp = vec;
        utility::BigFloat(tmp.vec[0]);
        utility::BigFloat(tmp.vec[1]);
        writeUBytes((atUint8*)&tmp, 8);
    }
    inline void writeValBig(const atVec2f& val) {writeVec2fBig(val);}

    /** @brief Writes an atVec3f (12 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec3f(const atVec3f& vec)
    {
        atVec3f tmp = vec;
        if (m_endian == BigEndian)
        {
            utility::BigFloat(tmp.vec[0]);
            utility::BigFloat(tmp.vec[1]);
            utility::BigFloat(tmp.vec[2]);
        }
        else
        {
            utility::LittleFloat(tmp.vec[0]);
            utility::LittleFloat(tmp.vec[1]);
            utility::LittleFloat(tmp.vec[2]);
        }
        writeUBytes((atUint8*)&tmp, 12);
    }
    inline void writeVal(const atVec3f& val) {writeVec3f(val);}

    /** @brief Writes an atVec3f (12 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec3fLittle(const atVec3f& vec)
    {
        atVec3f tmp = vec;
        utility::LittleFloat(tmp.vec[0]);
        utility::LittleFloat(tmp.vec[1]);
        utility::LittleFloat(tmp.vec[2]);
        writeUBytes((atUint8*)&tmp, 12);
    }
    inline void writeValLittle(const atVec3f& val) {writeVec3fLittle(val);}

    /** @brief Writes an atVec3f (12 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec3fBig(const atVec3f& vec)
    {
        atVec3f tmp = vec;
        utility::BigFloat(tmp.vec[0]);
        utility::BigFloat(tmp.vec[1]);
        utility::BigFloat(tmp.vec[2]);
        writeUBytes((atUint8*)&tmp, 12);
    }
    inline void writeValBig(const atVec3f& val) {writeVec3fBig(val);}

    /** @brief Writes an atVec4f (16 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec4f(const atVec4f& vec)
    {
        atVec4f tmp = vec;
        if (m_endian == BigEndian)
        {
            utility::BigFloat(tmp.vec[0]);
            utility::BigFloat(tmp.vec[1]);
            utility::BigFloat(tmp.vec[2]);
            utility::BigFloat(tmp.vec[3]);
        }
        else
        {
            utility::LittleFloat(tmp.vec[0]);
            utility::LittleFloat(tmp.vec[1]);
            utility::LittleFloat(tmp.vec[2]);
            utility::LittleFloat(tmp.vec[3]);
        }
        writeUBytes((atUint8*)&tmp, 16);
    }
    inline void writeVal(const atVec4f& val) {writeVec4f(val);}

    /** @brief Writes an atVec4f (16 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec4fLittle(const atVec4f& vec)
    {
        atVec4f tmp = vec;
        utility::LittleFloat(tmp.vec[0]);
        utility::LittleFloat(tmp.vec[1]);
        utility::LittleFloat(tmp.vec[2]);
        utility::LittleFloat(tmp.vec[3]);
        writeUBytes((atUint8*)&tmp, 16);
    }
    inline void writeValLittle(const atVec4f& val) {writeVec4fLittle(val);}

    /** @brief Writes an atVec4f (16 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec4fBig(const atVec4f& vec)
    {
        atVec4f tmp = vec;
        utility::BigFloat(tmp.vec[0]);
        utility::BigFloat(tmp.vec[1]);
        utility::BigFloat(tmp.vec[2]);
        utility::BigFloat(tmp.vec[3]);
        writeUBytes((atUint8*)&tmp, 16);
    }
    inline void writeValBig(const atVec4f& val) {writeVec4fBig(val);}

    /** @brief Writes an atVec2d (16 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     * @param vec The value to write to the buffer
     */
    inline void writeVec2d(const atVec2d& vec)
    {
        atVec2d tmp = vec;
        if (m_endian == BigEndian)
        {
            utility::BigDouble(tmp.vec[0]);
            utility::BigDouble(tmp.vec[1]);
        }
        else
        {
            utility::LittleDouble(tmp.vec[0]);
            utility::LittleDouble(tmp.vec[1]);
        }
        writeUBytes((atUint8*)&tmp, 16);
    }
    inline void writeVal(const atVec2d& val) {writeVec2d(val);}

    /** @brief Writes an atVec2d (16 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     * @param vec The value to write to the buffer
     */
    inline void writeVec2dLittle(const atVec2d& vec)
    {
        atVec2d tmp = vec;
        utility::LittleDouble(tmp.vec[0]);
        utility::LittleDouble(tmp.vec[1]);
        writeUBytes((atUint8*)&tmp, 16);
    }
    inline void writeValLittle(const atVec2d& val) {writeVec2dLittle(val);}

    /** @brief Writes an atVec2d (16 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     * @param vec The value to write to the buffer
     */
    inline void writeVec2dBig(const atVec2d& vec)
    {
        atVec2d tmp = vec;
        utility::BigDouble(tmp.vec[0]);
        utility::BigDouble(tmp.vec[1]);
        writeUBytes((atUint8*)&tmp, 16);
    }
    inline void writeValBig(const atVec2d& val) {writeVec2dBig(val);}

    /** @brief Writes an atVec3d (24 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec3d(const atVec3d& vec)
    {
        atVec3d tmp = vec;
        if (m_endian == BigEndian)
        {
            utility::BigDouble(tmp.vec[0]);
            utility::BigDouble(tmp.vec[1]);
            utility::BigDouble(tmp.vec[2]);
        }
        else
        {
            utility::LittleDouble(tmp.vec[0]);
            utility::LittleDouble(tmp.vec[1]);
            utility::LittleDouble(tmp.vec[2]);
        }
        writeUBytes((atUint8*)&tmp, 24);
    }
    inline void writeVal(const atVec3d& val) {writeVec3d(val);}

    /** @brief Writes an atVec3d (24 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec3dLittle(const atVec3d& vec)
    {
        atVec3d tmp = vec;
        utility::LittleDouble(tmp.vec[0]);
        utility::LittleDouble(tmp.vec[1]);
        utility::LittleDouble(tmp.vec[2]);
        writeUBytes((atUint8*)&tmp, 24);
    }
    inline void writeValLittle(const atVec3d& val) {writeVec3dLittle(val);}

    /** @brief Writes an atVec3d (24 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec3dBig(const atVec3d& vec)
    {
        atVec3d tmp = vec;
        utility::BigDouble(tmp.vec[0]);
        utility::BigDouble(tmp.vec[1]);
        utility::BigDouble(tmp.vec[2]);
        writeUBytes((atUint8*)&tmp, 24);
    }
    inline void writeValBig(const atVec3d& val) {writeVec3dBig(val);}

    /** @brief Writes an atVec4d (32 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec4d(const atVec4d& vec)
    {
        atVec4d tmp = vec;
        if (m_endian == BigEndian)
        {
            utility::BigDouble(tmp.vec[0]);
            utility::BigDouble(tmp.vec[1]);
            utility::BigDouble(tmp.vec[2]);
            utility::BigDouble(tmp.vec[3]);
        }
        else
        {
            utility::LittleDouble(tmp.vec[0]);
            utility::LittleDouble(tmp.vec[1]);
            utility::LittleDouble(tmp.vec[2]);
            utility::LittleDouble(tmp.vec[3]);
        }
        writeUBytes((atUint8*)&tmp, 32);
    }
    inline void writeVal(const atVec4d& val) {writeVec4d(val);}

    /** @brief Writes an atVec4d (32 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against little depending on the platform.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec4dLittle(const atVec4d& vec)
    {
        atVec4d tmp = vec;
        utility::LittleDouble(tmp.vec[0]);
        utility::LittleDouble(tmp.vec[1]);
        utility::LittleDouble(tmp.vec[2]);
        utility::LittleDouble(tmp.vec[3]);
        writeUBytes((atUint8*)&tmp, 32);
    }
    inline void writeValLittle(const atVec4d& val) {writeVec4dLittle(val);}

    /** @brief Writes an atVec4d (32 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes against big depending on the platform.
     *
     *  @param vec The value to write to the buffer
     */
    inline void writeVec4dBig(const atVec4d& vec)
    {
        atVec4d tmp = vec;
        utility::BigDouble(tmp.vec[0]);
        utility::BigDouble(tmp.vec[1]);
        utility::BigDouble(tmp.vec[2]);
        utility::BigDouble(tmp.vec[3]);
        writeUBytes((atUint8*)&tmp, 32);
    }
    inline void writeValBig(const atVec4d& val) {writeVec4dBig(val);}

    /** @brief Converts a UTF8 string to a wide-char string in the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param str The string to write to the buffer
     *  @param fixedLen If not -1, the number of characters to zero-fill string to
     *
     *  Endianness is set with setEndian
     */
    inline void writeStringAsWString(const std::string& str, atInt32 fixedLen = -1)
    {
        if (fixedLen == 0)
            return;
        std::string tmpStr = "\xEF\xBB\xBF" + str;
        const utf8proc_uint8_t* buf = reinterpret_cast<const utf8proc_uint8_t*>(tmpStr.c_str());
        if (fixedLen < 0)
        {
            while (*buf)
            {
                utf8proc_int32_t wc;
                utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
                if (len < 0)
                {
                    atWarning("invalid UTF-8 character while decoding");
                    return;
                }
                buf += len;
                if (wc != 0xFEFF)
                    writeUint16(atUint16(wc));
            }
            writeUint16(0);
        }
        else
        {
            for (atInt32 i=0 ; i<fixedLen ; ++i)
            {
                utf8proc_int32_t wc = 0;
                if (*buf)
                {
                    utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
                    if (len < 0)
                    {
                        atWarning("invalid UTF-8 character while decoding");
                        return;
                    }
                    buf += len;
                }

                if (wc == 0xFEFF)
                {
                    --i;
                    continue;
                }

                writeUint16(atUint16(wc));
            }
        }
    }

    /** @brief Converts a UTF8 string to a wide-char string in the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param str The string to write to the buffer
     *  @param fixedLen If not -1, the number of characters to zero-fill string to
     *
     *  Endianness is little
     */
    inline void writeStringAsWStringLittle(const std::string& str, atInt32 fixedLen = -1)
    {
        if (fixedLen == 0)
            return;
        std::string tmpStr = "\xEF\xBB\xBF" + str;
        const utf8proc_uint8_t* buf = reinterpret_cast<const utf8proc_uint8_t*>(tmpStr.c_str());
        if (fixedLen < 0)
        {
            while (*buf)
            {
                utf8proc_int32_t wc;
                utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
                if (len < 0)
                {
                    atWarning("invalid UTF-8 character while decoding");
                    return;
                }
                buf += len;
                if (wc != 0xFEFF)
                    writeUint16Little(atUint16(wc));
            }
            writeUint16Little(0);
        }
        else
        {
            for (atInt32 i=0 ; i<fixedLen ; ++i)
            {
                utf8proc_int32_t wc = 0;
                if (*buf)
                {
                    utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
                    if (len < 0)
                    {
                        atWarning("invalid UTF-8 character while decoding");
                        return;
                    }
                    buf += len;
                }

                if (wc == 0xFEFF)
                {
                    --i;
                    continue;
                }

                writeUint16Little(atUint16(wc));
            }
        }
    }

    /** @brief Converts a UTF8 string to a wide-char string in the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  @param str The string to write to the buffer
     *  @param fixedLen If not -1, the number of characters to zero-fill string to
     *
     *  Endianness is big
     */
    inline void writeStringAsWStringBig(const std::string& str, atInt32 fixedLen = -1)
    {
        if (fixedLen == 0)
            return;

        std::string tmpStr = "\xEF\xBB\xBF" + str;
        const utf8proc_uint8_t* buf = reinterpret_cast<const utf8proc_uint8_t*>(tmpStr.c_str());
        if (fixedLen < 0)
        {
            while (*buf)
            {
                utf8proc_int32_t wc;
                utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
                if (len < 0)
                {
                    atWarning("invalid UTF-8 character while decoding");
                    return;
                }
                buf += len;
                if (wc != 0xFEFF)
                    writeUint16Big(atUint16(wc));
            }
            writeUint16Big(0);
        }
        else
        {
            for (atInt32 i=0 ; i<fixedLen ; ++i)
            {
                utf8proc_int32_t wc = 0;
                if (*buf)
                {
                    utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
                    if (len < 0)
                    {
                        atWarning("invalid UTF-8 character while decoding");
                        return;
                    }
                    buf += len;
                }

                if (wc == 0xFEFF)
                {
                    --i;
                    continue;
                }

                writeUint16Big(atUint16(wc));
            }
        }
    }

    /** @brief Writes an string to the buffer and advances the buffer.
     *
     *  @param str The string to write to the buffer
     *  @param fixedLen If not -1, the number of characters to zero-fill string to
     */
    inline void writeString(const std::string& str, atInt32 fixedLen = -1)
    {
        if (fixedLen == 0)
            return;

        if (fixedLen < 0)
        {
            for (atUint8 c : str)
            {
                writeUByte(c);

                if (c == '\0')
                    break;
            }
            writeUByte(0);
        }
        else
        {
            auto it = str.begin();
            for (atInt32 i=0 ; i<fixedLen ; ++i)
            {
                atUint8 chr;
                if (it == str.end())
                    chr = 0;
                else
                    chr = *it++;
                writeUByte(chr);
            }
        }
    }
    inline void writeVal(const std::string& val) {writeString(val);}

    /** @brief Writes an wstring to the buffer and advances the buffer.
     *
     *  @param str The string to write to the buffer
     *  @param fixedLen If not -1, the number of characters to zero-fill string to
     *
     *  Endianness is set with setEndian
     */
    inline void writeWString(const std::wstring& str, atInt32 fixedLen = -1)
    {
        if (fixedLen == 0)
            return;

        if (fixedLen < 0)
        {
            for (atUint16 c : str)
            {
                writeUint16(c);

                if (c == L'\0')
                    break;
            }
            writeUint16(0);
        }
        else
        {
            auto it = str.begin();
            for (atInt32 i=0 ; i<fixedLen ; ++i)
            {
                atUint16 chr;
                if (it == str.end())
                    chr = 0;
                else
                    chr = *it++;
                writeUint16(chr);
            }
        }
    }
    inline void writeVal(const std::wstring& val) {writeWString(val);}

    /** @brief Writes an wstring to the buffer and advances the buffer.
     *
     *  @param str The string to write to the buffer
     *  @param fixedLen If not -1, the number of characters to zero-fill string to
     *
     *  Endianness is little
     */
    inline void writeWStringLittle(const std::wstring& str, atInt32 fixedLen = -1)
    {
        if (fixedLen == 0)
            return;

        if (fixedLen < 0)
        {
            for (atUint16 c : str)
            {
                writeUint16Little(c);

                if (c == L'\0')
                    break;
            }
            writeUint16Little(0);
        }
        else
        {
            auto it = str.begin();
            for (atInt32 i=0 ; i<fixedLen ; ++i)
            {
                atUint16 chr;
                if (it == str.end())
                    chr = 0;
                else
                    chr = *it++;
                writeUint16Little(chr);
            }
        }
    }
    inline void writeValLittle(const std::wstring& val) {writeWStringLittle(val);}

    /** @brief Writes an wstring to the buffer and advances the buffer.
     *
     *  @param str The string to write to the buffer
     *  @param fixedLen If not -1, the number of characters to zero-fill string to
     *
     *  Endianness is big
     */
    inline void writeWStringBig(const std::wstring& str, atInt32 fixedLen = -1)
    {
        if (fixedLen == 0)
            return;

        if (fixedLen < 0)
        {
            for (atUint16 c : str)
            {
                writeUint16Big(c);

                if (c == L'\0')
                    break;
            }
            writeUint16Big(0);
        }
        else
        {
            auto it = str.begin();
            for (atInt32 i=0 ; i<fixedLen ; ++i)
            {
                atUint16 chr;
                if (it == str.end())
                    chr = 0;
                else
                    chr = *it++;
                writeUint16Big(chr);
            }
        }
    }
    inline void writeValBig(const std::wstring& val) {writeWStringBig(val);}

    inline void fill(atUint8 val, atUint64 length)
    {for (atUint64 l=0 ; l<length ; ++l) writeUBytes(&val, 1);}
    inline void fill(atInt8 val, atUint64 length)
    {fill((atUint8)val, length);}

    /** @brief Performs automatic std::vector enumeration writes using numeric type T
     *  @param vector The std::vector read from when writing data
     *
     *  Endianness is set with setEndian
     */
    template <class T>
    void enumerate(const std::vector<T>& vector,
                   typename std::enable_if<std::is_arithmetic<T>::value ||
                   std::is_same<T, atVec2f>::value ||
                   std::is_same<T, atVec3f>::value ||
                   std::is_same<T, atVec4f>::value>::type* = 0)
    {
        for (const T& item : vector)
            writeVal(item);
    }

    /** @brief Performs automatic std::vector enumeration writes using numeric type T
     *  @param vector The std::vector read from when writing data
     *
     *  Endianness is little
     */
    template <class T>
    void enumerateLittle(const std::vector<T>& vector,
                         typename std::enable_if<std::is_arithmetic<T>::value ||
                         std::is_same<T, atVec2f>::value ||
                         std::is_same<T, atVec3f>::value ||
                         std::is_same<T, atVec4f>::value>::type* = 0)
    {
        for (const T& item : vector)
            writeValLittle(item);
    }

    /** @brief Performs automatic std::vector enumeration writes using numeric type T
     *  @param vector The std::vector read from when writing data
     *
     *  Endianness is big
     */
    template <class T>
    void enumerateBig(const std::vector<T>& vector,
                      typename std::enable_if<std::is_arithmetic<T>::value ||
                      std::is_same<T, atVec2f>::value ||
                      std::is_same<T, atVec3f>::value ||
                      std::is_same<T, atVec4f>::value>::type* = 0)
    {
        for (const T& item : vector)
            writeValBig(item);
    }

    /** @brief Performs automatic std::vector enumeration writes using non-numeric type T
     *  @param vector The std::vector read from when writing data
     */
    template <class T>
    void enumerate(const std::vector<T>& vector,
                   typename std::enable_if<!std::is_arithmetic<T>::value &&
                   !std::is_same<T, atVec2f>::value &&
                   !std::is_same<T, atVec3f>::value &&
                   !std::is_same<T, atVec4f>::value>::type* = 0)
    {
        for (const T& item : vector)
            item.write(*this);
    }

protected:
    Endian      m_endian;
};

template <typename T>
IStreamWriter& operator<<(IStreamWriter& lhs, const T& rhs)
{
    lhs.writeVal(rhs);
    return lhs;
}
}
}
#endif // STREAMWRITER_HPP

