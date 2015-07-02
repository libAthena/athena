#ifndef MEMORYREADER_HPP
#define MEMORYREADER_HPP

#include "Athena/IStreamReader.hpp"
#include <string>
#include <functional>

namespace Athena
{
namespace io
{
/*! \class BinaryReader
 *  \brief A Stream class for reading binary data
 *
 *  A Class for reading binary data from a file or memory stream,
 *  all work is done using a memory buffer, and not read directly from the disk
 *  this allows for fast, flexible code as well as the ability to quickly modify data
 *  \sa Stream
 */
class MemoryReader : public IStreamReader
{
public:
    /*! \brief This constructor takes an existing buffer to read from.
     *
     *   \param data The existing buffer
     *   \param length The length of the existing buffer
     */
    MemoryReader(const atUint8* data, atUint64 length);

    /*! \brief This constructor creates an instance from a file on disk.
     *
     * \param filename The file to create the stream from
     */
    MemoryReader(const std::string& filename, std::function<void(int)> progressFun = nullptr);

    virtual ~MemoryReader();

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
    inline bool atEnd() const
    {return m_position >= m_length;}

    /*! \brief Returns the current position in the stream.
     *
     *  \return Int64 The current position in the stream.
     */
    inline atUint64 position() const
    {return m_position;}

    /*! \brief Returns whether or not the stream is at the end.
     *
     *  \return bool True if at end; False otherwise.
     */
    inline atUint64 length() const
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
     *  \sa Endian
     *  \param filepath The path to write to.
     */
    inline void setFilepath(const std::string& filepath)
    {m_filepath = filepath;}

    /*! \brief Returns the target file
     *
     */
    inline std::string filepath() const
    {return m_filepath;}


    /*!
     * \brief Seeks to the specified bit within the current byte
     * \param bit Bit to seek to, range is 0 - 7
     */
    void seekBit(int bit);

    /*! \brief Reads a bit at the current position and advances the current position
     *
     * \return bool The value at the current position
     */
    bool readBit();

    /*! \brief Reads a byte at the current position and advances the current position
     *
     * \return Int8 The value at the current position
     */
    atInt8 readByte();

    /*! \brief Reads a byte at the current position and advances the current position
     *
     * \return Uint8 The value at the current position
     */
    atUint8 readUByte();

    /*! \brief Reads a byte at the current position and advances the current position.
     *
     * \return Uint8* The buffer at the current position from the given length.
     */
    inline atInt8* readBytes(atUint64 length) {return (atInt8*)readUBytes(length);}

    /*! \brief Reads a byte at the current position and advances the current position.
     *
     * \return Int8* The buffer at the current position from the given length.
     */
    atUint8* readUBytes(atUint64 length);

    atUint64 readBytesToBuf(void* buf, atUint64 len) {return readUBytesToBuf(buf, len);}
    atUint64 readUBytesToBuf(void* buf, atUint64 len);

    /*! \brief Reads a Int16 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int16 The value at the current address
     *  \throw IOException when address is out of range
     */
    atInt16 readInt16();

    /*! \brief Reads a Uint16 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint16 The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atUint16 readUint16()
    {return MemoryReader::readInt16();}

    /*! \brief Reads a Int32 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int32 The value at the current address
     *  \throw IOException when address is out of range
     */
    atInt32 readInt32();

    /*! \brief Reads a Uint32 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint32 The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atUint32 readUint32()
    {return MemoryReader::readInt32();}

    /*! \brief Reads a Int64 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Int64 The value at the current address
     *  \throw IOException when address is out of range
     */
    atInt64 readInt64();

    /*! \brief Reads a Uint64 and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return Uint64 The value at the current address
     *  \throw IOException when address is out of range
     */
    inline atUint64 readUint64()
    {return MemoryReader::readInt64();}

    /*! \brief Reads a float and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return float The value at the current address
     *  \throw IOException when address is out of range
     */
    float readFloat();

    /*! \brief Reads a double and swaps to proper endianness depending on platform
     *  and Stream settings, and advances the current position
     *
     *  \sa Endian
     *
     *  \return double The value at the current address
     *  \throw IOException when address is out of range
     */
    double readDouble();

    /*! \brief Reads a bool and advances the current position
     *
     *  \return bool The value at the current address
     *  \throw IOException when address is out of range
     */
    bool readBool();

    /*! \brief Reads an atVec3f (12 bytes) and advances the current position
     *
     *  \return atVec3f The value at the current address
     *  \throw IOException when address is out of range
     */
    atVec3f readVec3f();

    /*! \brief Reads an atVec4f (16 bytes) and advances the current position
     *
     *  \return atVec4f The value at the current address
     *  \throw IOException when address is out of range
     */
    atVec4f readVec4f();

    /*! \brief Reads a Unicode string and advances the position in the file
     *
     *  \param fixedLen If non-negative, this is a fixed-length string read
     *  \return std::string The value at the current address
     *  \throw IOException when address is out of range
     */
    std::string readUnicode(atInt32 fixedLen = -1);

    /*! \brief Reads a string and advances the position in the file
     *
     *  \param fixedLen If non-negative, this is a fixed-length string read
     *  \return std::string The value at the current address
     *  \throw IOException when address is out of range
     */
    std::string readString(atInt32 fixedLen = -1);

    /*! \brief Reads a wstring and advances the position in the file
     *
     *  \param fixedLen If non-negative, this is a fixed-length string read
     *  \return std::wstring The value at the current address
     *  \throw IOException when address is out of range
     */
    std::wstring readWString(atInt32 fixedLen = -1);

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
};

} // io
} // Athena

#ifndef MEMORYREADER_BASE
#define MEMORYREADER_BASE() \
private: \
    typedef Athena::io::MemoryReader base

#endif // MEMORYREADER_BASE

#endif // MEMORYREADER_HPP
