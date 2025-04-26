#pragma once

#include <string>
#include <memory>
#include <functional>
#include "athena/IStreamReader.hpp"

namespace athena::io {
/*! \class MemoryReader
 *  \brief A Stream class for reading data from a memory position
 *
 *  A Class for reading binary data from a file or memory stream,
 *  all work is done using a memory buffer, and not read directly from the disk
 *  this allows for fast, flexible code as well as the ability to quickly modify data
 *  \sa Stream
 */
class MemoryReader : public IStreamReader {
protected:
  MemoryReader() = default;

public:
  ~MemoryReader() override;

  /*! \brief This constructor references an existing buffer to read from.
   *
   *   \param data          The existing buffer.
   *   \param length        The length of the existing buffer.
   *   \param takeOwnership Memory will be freed with the reader if set.
   *   \param globalErr     Whether or not global errors are enabled.
   */
  explicit MemoryReader(const void* data, uint64_t length, bool takeOwnership = false, bool globalErr = true);

  /*! \brief Sets the buffers position relative to the specified position.<br />
   *         It seeks relative to the current position by default.
   *  \param position where in the buffer to seek
   *  \param origin The Origin to seek \sa SeekOrigin
   */
  void seek(int64_t position, SeekOrigin origin = SeekOrigin::Current) override;

  /*! \brief Returns the current position in the stream.
   *
   *  \return Int64 The current position in the stream.
   */
  uint64_t position() const override { return m_position; }

  /*! \brief Returns whether or not the stream is at the end.
   *
   *  \return bool True if at end; False otherwise.
   */
  uint64_t length() const override { return m_length; }

  /*! \brief Sets the buffer to the given one, deleting the current one.<br />
   *         <b>BEWARE:</b> As this deletes the current buffer it WILL cause a loss of data
   *         if that was not the intent.<br />
   *         Once you pass the data to setData <b>DO NOT</b> delete the buffer
   *         as Stream now owns the address, this is done to keep memory usage down.
   *  \param data The new buffer.
   *  \param length The length of the new buffer.
   *  \param takeOwnership Memory will be freed with the reader if set
   *  \throw IOException
   */
  void setData(const uint8_t* data, uint64_t length, bool takeOwnership = false);

  /*! \brief Returns a copy of the current buffer.<br />
   *         Changes to the copy do not affect the buffer so it's perfectly safe to
   *         directly edit the buffer and use setData to set the new information.<br />
   *         However once you pass the data to setData <b>DO NOT</b> delete the buffer
   *         as Stream now owns the address, this is done to keep memory usage down.
   *  \return Uint8* The copy of the buffer.
   */
  uint8_t* data() const;

  /*! \brief Reads a specified number of bytes to user-allocated buffer
   *  \param buf User-allocated buffer pointer
   *  \param len Length to read
   *  \return Number of bytes read
   */
  uint64_t readUBytesToBuf(void* buf, uint64_t len) override;

protected:
  const void* m_data = nullptr;
  uint64_t m_length = 0;
  uint64_t m_position = 0;
  bool m_owns = false;
  bool m_globalErr = true;
};

class MemoryCopyReader : public MemoryReader {
public:
  /*! \brief This constructor copies an existing buffer to read from.
   *
   *   \param data The existing buffer
   *   \param length The length of the existing buffer
   */
  explicit MemoryCopyReader(const void* data, uint64_t length);

  /*! \brief This constructor creates an instance from a file on disk.
   *
   * \param filename The file to create the stream from
   */
  explicit MemoryCopyReader(const std::string& filename) : m_filepath(filename) { loadData(); }

  void setData(const uint8_t* data, uint64_t length);

protected:
  void loadData();
  std::unique_ptr<uint8_t[]> m_dataCopy;
  std::string m_filepath; //!< Path to the target file
};

} // namespace athena::io
