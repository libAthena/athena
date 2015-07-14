#ifndef ISTREAMWRITER_HPP
#define ISTREAMWRITER_HPP

#include <locale>
#include <codecvt>
#include "IStream.hpp"

namespace Athena
{
namespace io
{
class IStreamWriter : public IStream
{
public:
    virtual ~IStreamWriter() {}
    /*! \brief Sets the Endianness of the stream
     *
     *  \param endian The Endianness to set \sa Endian
     */
    inline void setEndian(Endian endian)
    {m_endian = endian;}

    /*! \brief Returns the current Endianness of the stream
     *
     *  \return Endian The current Stream Endianness
     */
    inline Endian endian() const
    {return m_endian;}

    /*! \brief Returns whether the stream is BigEndian
     *
     *  \return bool True for BigEndian; False for LittleEndian
     */
    inline bool isBigEndian() const
    {return (m_endian == Endian::BigEndian);}

    /*! \brief Returns whether the stream is LittleEndian
     *
     *  \return bool True for LittleEndian; False for BigEndian
     */
    inline bool isLittleEndian() const
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
    inline bool atEnd() const {return position() >= length();}

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

    /*! \brief Writes a byte at the current position and advances the position by one byte.
     * \param byte The value to write
     */
    inline void writeUByte(atUint8 val) {writeUBytes(&val, 1);}

    /*! \brief Writes a byte at the current position and advances the position by one byte.
     * \param byte The value to write
     * \throw IOException
     */
    inline void writeByte(atInt8 val) {writeUByte(val);}

    /*! \brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     * \param data The buffer to write
     * \param length The amount to write
     */
    virtual void writeUBytes(const atUint8* data, atUint64 len)=0;

    /*! \brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     * \param data The buffer to write
     * \param length The amount to write
     */
    inline void writeBytes(const atInt8* data, atUint64 len) {writeUBytes((atUint8*)data, len);}

    /*! \brief Writes an Int16 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeInt16(atInt16 val)
    {
        if (m_endian == BigEndian)
            utility::BigInt16(val);
        else
            utility::LittleInt16(val);
        writeUBytes((atUint8*)&val, 2);
    }

    /*! \brief Writes an Uint16 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeUint16(atUint16 val) {writeInt16(val);}

    /*! \brief Writes an Int32 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeInt32(atInt32 val)
    {
        if (m_endian == BigEndian)
            utility::BigInt32(val);
        else
            utility::LittleInt32(val);
        writeUBytes((atUint8*)&val, 4);
    }

    /*! \brief Writes an Uint32 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeUint32(atUint32 val) {writeInt32(val);}

    /*! \brief Writes an Int64 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeInt64(atInt64 val)
    {
        if (m_endian == BigEndian)
            utility::BigInt64(val);
        else
            utility::LittleInt64(val);
        writeUBytes((atUint8*)&val, 8);
    }

    /*! \brief Writes an Uint64 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeUint64(atUint64 val) {writeInt64(val);}

    /*! \brief Writes an float to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeFloat(float val)
    {
        if (m_endian == BigEndian)
            utility::BigFloat(val);
        else
            utility::LittleFloat(val);
        writeUBytes((atUint8*)&val, 4);
    }

    /*! \brief Writes an double to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeDouble(double val)
    {
        if (m_endian == BigEndian)
            utility::BigDouble(val);
        else
            utility::LittleDouble(val);
        writeUBytes((atUint8*)&val, 8);
    }

    /*! \brief Writes an bool to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeBool(bool val) {writeUBytes((atUint8*)&val, 1);}

    /*! \brief Writes an atVec2f (8 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     * \sa Endian
     * \param vec The value to write to the buffer
     */
    inline void writeVec2f(atVec2f vec)
    {
        if (m_endian == BigEndian)
        {
            utility::BigFloat(vec.vec[0]);
            utility::BigFloat(vec.vec[1]);
        }
        else
        {
            utility::LittleFloat(vec.vec[0]);
            utility::LittleFloat(vec.vec[1]);
        }
        writeUBytes((atUint8*)&vec, 8);
    }

    /*! \brief Writes an atVec3f (12 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     * \sa Endian
     * \param vec The value to write to the buffer
     */
    inline void writeVec3f(atVec3f vec)
    {
        if (m_endian == BigEndian)
        {
            utility::BigFloat(vec.vec[0]);
            utility::BigFloat(vec.vec[1]);
            utility::BigFloat(vec.vec[2]);
        }
        else
        {
            utility::LittleFloat(vec.vec[0]);
            utility::LittleFloat(vec.vec[1]);
            utility::LittleFloat(vec.vec[2]);
        }
        writeUBytes((atUint8*)&vec, 12);
    }

    /*! \brief Writes an atVec4f (16 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     * \sa Endian
     * \param vec The value to write to the buffer
     */
    inline void writeVec4f(atVec4f vec)
    {
        if (m_endian == BigEndian)
        {
            utility::BigFloat(vec.vec[0]);
            utility::BigFloat(vec.vec[1]);
            utility::BigFloat(vec.vec[2]);
            utility::BigFloat(vec.vec[3]);
        }
        else
        {
            utility::LittleFloat(vec.vec[0]);
            utility::LittleFloat(vec.vec[1]);
            utility::LittleFloat(vec.vec[2]);
            utility::LittleFloat(vec.vec[3]);
        }
        writeUBytes((atUint8*)&vec, 16);
    }

    /*! \brief Writes an unicode string to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param str The string to write to the buffer
     *  \param fixedLen If not -1, the number of characters to zero-fill string to
     */
    inline void writeUnicode(const std::string& str, atInt32 fixedLen = -1)
    {
        std::string tmpStr = "\xEF\xBB\xBF" + str;

        std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
        std::wstring tmp = conv.from_bytes(tmpStr);

        if (fixedLen < 0)
        {
            for (atUint16 chr : tmp)
            {
                if (chr != 0xFEFF)
                    writeUint16(chr);
            }
            writeUint16(0);
        }
        else
        {
            auto it = tmp.begin();
            for (atInt32 i=0 ; i<fixedLen ; ++i)
            {
                atUint16 chr;
                if (it == tmp.end())
                    chr = 0;
                else
                    chr = *it++;

                if (chr == 0xFEFF)
                {
                    --i;
                    continue;
                }

                writeUint16(chr);
            }
        }
    }

    /*! \brief Writes an string to the buffer and advances the buffer.
     *
     *  \sa Endian
     *  \param str The string to write to the buffer
     *  \param fixedLen If not -1, the number of characters to zero-fill string to
     */
    inline void writeString(const std::string& str, atInt32 fixedLen = -1)
    {
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

    /*! \brief Writes an wstring to the buffer and advances the buffer.
     *
     *  \sa Endian
     *  \param str The string to write to the buffer
     *  \param fixedLen If not -1, the number of characters to zero-fill string to
     */
    inline void writeWString(const std::wstring& str, atInt32 fixedLen = -1)
    {
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

    inline void fill(atUint8 val, atUint64 length)
    {for (atUint64 l=0 ; l<length ; ++l) writeUBytes(&val, 1);}
    inline void fill(atInt8 val, atUint64 length)
    {fill((atUint8)val, length);}

protected:
    Endian      m_endian;
};
}
}
#endif // STREAMWRITER_HPP

