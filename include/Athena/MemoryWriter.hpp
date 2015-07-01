#ifndef MEMORYWRITER_HPP
#define MEMORYWRITER_HPP

#include "Athena/IStreamWriter.hpp"
#include <string>
#include <functional>

namespace Athena
{
namespace io
{

/*! \class BinaryWriter
 *  \brief A Stream class for writing binary data
 *
 *  A Class for writing binary data to a file or memory stream,
 *  all work is done using a memory buffer, and not written directly to the disk
 *  this allows for fast, flexible code as well as the ability to quickly modify data
 *  \sa Stream
 */
class MemoryWriter : public IStreamWriter
{
public:
    /*! \brief This constructor takes an existing buffer to write to.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    explicit MemoryWriter(atUint8* data = nullptr, atUint64 length = 0x10);

    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    MemoryWriter(const std::string& filename, std::function<void(int)> progressFun = nullptr);

    virtual ~MemoryWriter();

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

    /*! \brief Retuns whether or not the Stream is open.
     *
     *  \return True if open; False otherwise.
     */
    inline bool isOpen() const
    {return m_data != nullptr;}


    /*! \brief Sets the buffers position relative to the specified position.<br />
     *         It seeks relative to the current position by default.
     *  \param position where in the buffer to seek
     *  \param origin The Origin to seek \sa SeekOrigin
     */
    void seek(atInt64 pos, SeekOrigin origin = SeekOrigin::Current);

    /*! \brief Sets the buffers position relative to the next 32-byte aligned position.<br />
     */
    inline void seekAlign32() {seek(ROUND_UP_32(m_position), SeekOrigin::Begin);}


    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    inline bool atEnd()     const
    {return m_position >= m_length;}


    /*! \brief Returns the current position in the stream.
     *
     *  \return Int64 The current position in the stream.
     */
    inline atUint64 position()    const
    {return m_position;}

    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    inline atUint64 length()      const
    {return m_length;}

    /*! \brief Sets the buffer to the given one, deleting the current one.<br />
     *         <b>BEWARE:</b> As this deletes the current buffer it WILL cause a loss of data
     *         if that was not the intent.<br />
     *         Once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \param data The new buffer.
     *  \param length The length of the new buffer.
     *  \throw IOException
     */
    void  setData(const atUint8* data, atUint64 length);


    /*! \brief Returns a copy of the current buffer.<br />
     *         Changes to the copy do not affect the buffer so it's perfectly safe to
     *         directly edit the buffer and use setData to set the new information.<br />
     *         However once you pass the data to setData <b>DO NOT</b> delete the buffer
     *         as Stream now owns the address, this is done to keep memory usage down.
     *  \return Uint8* The copy of the buffer.
     */
    atUint8* data() const;

    /*! \brief Sets the target file
     *
     *  \param filepath The path to write to.
     */
    inline void setFilepath(const std::string& filepath)
    {m_filepath = filepath;}

    /*! \brief
     * Returns the target file
     */
    inline std::string filepath() const
    {return m_filepath;}


    /*! \brief Saves the file to the specified file.
     *
     *   \param filename If not empty, the filename to save to
     */
    void save(const std::string& filename = "");

    /*!
     * \brief Seeks to the specified bit within the current byte
     * \param bit Bit to seek to, range is 0 - 7
     */
    void seekBit(int bit);

    /*! \brief Writes a bit at the current position and advances the position by one bit.
     * \param val the value to write
     * \throw IOException
     */
    void writeBit(bool val);

    /*! \brief Writes a byte at the current position and advances the position by one byte.
     * \param byte The value to write
     */
    void writeUByte(atUint8 val);

    /*! \brief Writes a byte at the current position and advances the position by one byte.
     * \param byte The value to write
     * \throw IOException
     */
    inline void writeByte(atInt8 val)
    {MemoryWriter::writeUByte(val);}

    /*! \brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     * \param data The buffer to write
     * \param length The amount to write
     */
    void writeUBytes(const atUint8* data, atUint64 len);

    /*! \brief Writes the given buffer with the specified length, buffers can be bigger than the length
     *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
     *
     * \param data The buffer to write
     * \param length The amount to write
     */
    inline void writeBytes(const atInt8* data, atUint64 len)
    {MemoryWriter::writeUBytes((atUint8*)data, len);}

    /*! \brief Writes an Int16 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeInt16(atInt16 val);

    /*! \brief Writes an Uint16 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeUint16(atUint16 val)
    {MemoryWriter::writeInt16(val);}

    /*! \brief Writes an Int32 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeInt32(atInt32);

    /*! \brief Writes an Uint32 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeUint32(atUint32 val)
    {MemoryWriter::writeInt32(val);}

    /*! \brief Writes an Int64 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeInt64(atInt64);

    /*! \brief Writes an Uint64 to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    inline void writeUint64(atUint64 val)
    {MemoryWriter::writeInt64(val);}

    /*! \brief Writes an float to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeFloat(float);

    /*! \brief Writes an double to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeDouble(double);

    /*! \brief Writes an bool to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param val The value to write to the buffer
     */
    void writeBool(bool);

    /*! \brief Writes an atVec3f (12 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     * \sa Endian
     * \param vec The value to write to the buffer
     */
    void writeVec3f(atVec3f vec);

    /*! \brief Writes an atVec4f (16 bytes) to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     * \sa Endian
     * \param vec The value to write to the buffer
     */
    void writeVec4f(atVec4f vec);

    /*! \brief Writes an unicode string to the buffer and advances the buffer.
     *         It also swaps the bytes depending on the platform and Stream settings.
     *
     *  \sa Endian
     *  \param str The string to write to the buffer
     *  \param fixedLen If not -1, the number of characters to zero-fill string to
     */
    void writeUnicode(const std::string& str, atInt32 fixedLen = -1);

    /*! \brief Writes an string to the buffer and advances the buffer.
     *
     *  \sa Endian
     *  \param str The string to write to the buffer
     *  \param fixedLen If not -1, the number of characters to zero-fill string to
     */
    void writeString(const std::string& str, atInt32 fixedLen = -1);

    /*! \brief Writes an wstring to the buffer and advances the buffer.
     *
     *  \sa Endian
     *  \param str The string to write to the buffer
     *  \param fixedLen If not -1, the number of characters to zero-fill string to
     */
    void writeWString(const std::wstring& str, atInt32 fixedLen = -1);


    void fill(atUint8 val, atUint64 length);
    inline void fill(atInt8 val, atUint64 length)
    {MemoryWriter::fill((atUint8)val, length);}

    inline void setProgressCallback(std::function<void(int)> cb)
    {m_progressCallback = cb;}
protected:
    void loadData();
    atUint8*      m_data;
    atUint64      m_length;
    std::string m_filepath; //!< Path to the target file
    atUint64      m_position;
    atUint64      m_bitPosition;
    Endian      m_endian;
    std::function<void(int)> m_progressCallback;
private:
    void resize(atUint64 newSize);
};
}
}

#ifndef MEMORYWRITER_BASE
#define MEMORYWRITER_BASE() \
    private: \
        typedef Athena::io::MemoryWriter base
#endif // BINARYWRITER_BASE
#endif // MEMORYWRITER_HPP
