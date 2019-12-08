#pragma once

#include <cstdint>
#include <vector>

#include "athena/IStreamWriter.hpp"
#include "athena/Types.hpp"

namespace athena::io {

/*! @class VectorWriter
 *  @brief A Stream class for writing data to a STL vector
 *
 *  A Class for writing binary data to an STL vector,
 *  all work is done using a std::vector, and not written directly to the disk
 *  @sa Stream
 */
class VectorWriter : public IStreamWriter {
public:
  /*! @brief Sets the buffers position relative to the specified position.<br />
   *         It seeks relative to the current position by default.
   *  @param position where in the buffer to seek
   *  @param origin The Origin to seek @sa SeekOrigin
   */
  void seek(atInt64 position, SeekOrigin origin = SeekOrigin::Current) override;

  /*! @brief Returns the current position in the stream.
   *
   *  @return Int64 The current position in the stream.
   */
  atUint64 position() const override { return m_position; }

  /*! @brief Returns the length of the stream.
   *
   *  @return Int64 The length of the stream.
   */
  atUint64 length() const override { return m_data.size(); }

  bool isOpen() const { return true; }

  /*! @brief Obtains reference to underlying std::vector store */
  const std::vector<uint8_t>& data() const { return m_data; }

  /*! @brief Writes the given buffer with the specified length, buffers can be bigger than the length
   *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
   *  If you are needing to fill in an area please use @sa IStreamWriter::fill(atUint64) instead.
   *
   * @param data The buffer to write
   * @param length The amount to write
   */
  void writeUBytes(const atUint8* data, atUint64 length) override;

protected:
  std::vector<uint8_t> m_data;
  atUint64 m_position = 0;
};

} // namespace athena::io
