#pragma once

#include <memory>
#include <string>

#include "athena/IStreamWriter.hpp"

namespace athena::io {

/*! @class MemoryWriter
 *  @brief A Stream class for writing data to a memory position
 *
 *  A Class for writing binary data to a file or memory stream,
 *  all work is done using a memory buffer, and not written directly to the disk
 *  this allows for fast, flexible code as well as the ability to quickly modify data
 *  @sa Stream
 */
class MemoryWriter : public IStreamWriter {
public:
  ~MemoryWriter() override;

  /*! @brief This constructor references an existing buffer to write to in-place.
   *
   *   @param data          The existing buffer.
   *   @param length        The length of the existing buffer.
   *   @param takeOwnership Whether or not this writer takes ownership of the supplied data buffer.
   *                        If true, the buffer will be deleted by this when the destructor executes.
   */
  explicit MemoryWriter(uint8_t* data, uint64_t length, bool takeOwnership = false);

  /*! @brief Sets the buffers position relative to the specified position.<br />
   *         It seeks relative to the current position by default.
   *  @param position where in the buffer to seek
   *  @param origin The Origin to seek @sa SeekOrigin
   */
  void seek(int64_t position, SeekOrigin origin = SeekOrigin::Current) override;

  /*! @brief Returns the current position in the stream.
   *
   *  @return Int64 The current position in the stream.
   */
  uint64_t position() const override { return m_position; }

  /*! @brief Returns the length of the stream.
   *
   *  @return Int64 The length of the stream.
   */
  uint64_t length() const override { return m_length; }

  bool isOpen() const { return true; }

  /** @brief Sets the buffer to the given one, deleting the current one if it owns it.<br />
   *  @param data The new buffer.
   *  @param length The length of the new buffer.
   *  @param takeOwnership Whether the Stream now owns the buffer.
   */
  void setData(uint8_t* data, uint64_t length, bool takeOwnership = false);

  /*! @brief Returns a copy of the current buffer.<br />
   *         Changes to the copy do not affect the buffer so it's perfectly safe to
   *         directly edit the buffer and use setData to set the new information.<br />
   *  @return Uint8* The copy of the buffer.
   */
  uint8_t* data() const;

  /*! @brief Sets the target file
   *
   *  @param filepath The path to write to.
   */
  void setFilepath(const std::string& filepath) { m_filepath = filepath; }

  /*! @brief
   * Returns the target file
   */
  std::string filepath() const { return m_filepath; }

  /*! @brief Saves the file to the specified file.
   *
   *   @param filename If not empty, the filename to save to
   */
  void save(std::string_view filename = {});

  /*! @brief Writes the given buffer with the specified length, buffers can be bigger than the length
   *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
   *  If you are needing to fill in an area please use IStreamWriter::fill(atUint64) instead.
   *
   * @param data The buffer to write
   * @param length The amount to write
   */
  void writeUBytes(const uint8_t* data, uint64_t length) override;

protected:
  MemoryWriter() = default;
  uint8_t* m_data = nullptr;
  uint64_t m_length = 0;
  uint64_t m_position = 0;
  bool m_bufferOwned = false;
  std::string m_filepath; //!< Path to the target file
};

class MemoryCopyWriter : public MemoryWriter {
public:
  /*! @brief This constructor copies an existing buffer to write to.
   *
   *   @param data The existing buffer
   *   @param length The length of the existing buffer
   */
  explicit MemoryCopyWriter(uint8_t* data = nullptr, uint64_t length = 0x10);

  /*! @brief This constructor creates an instance from a file on disk.
   *
   * @param filename The file to create the stream from
   */
  explicit MemoryCopyWriter(std::string_view filename);

  /*! @brief Sets the buffers position relative to the specified position.<br />
   *         It seeks relative to the current position by default.
   *  @param position where in the buffer to seek
   *  @param origin The Origin to seek @sa SeekOrigin
   */
  void seek(int64_t position, SeekOrigin origin = SeekOrigin::Current) override;

  /*! @brief Sets the buffer to the given one, deleting the current one.<br />
   *         <b>BEWARE:</b> As this deletes the current buffer it WILL cause a loss of data
   *         if that was not the intent.<br />
   *         Once you pass the data to setData <b>DO NOT</b> delete the buffer
   *         as Stream now owns the address, this is done to keep memory usage down.
   *  @param data The new buffer.
   *  @param length The length of the new buffer.
   *  @throw IOException
   */
  void setData(const uint8_t* data, uint64_t length);

  /*! @brief Writes the given buffer with the specified length, buffers can be bigger than the length
   *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
   *
   * @param data The buffer to write
   * @param length The amount to write
   */
  void writeUBytes(const uint8_t* data, uint64_t length) override;

protected:
  std::unique_ptr<uint8_t[]> m_dataCopy;

private:
  void resize(uint64_t newSize);
};

} // namespace athena::io
