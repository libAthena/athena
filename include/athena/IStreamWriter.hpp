#pragma once

#include <string>
#include <type_traits>
#include <vector>

#include "utf8proc.h"

#include "athena/IStream.hpp"
#include "athena/Utility.hpp"

namespace athena::io {
class IStreamWriter : public IStream {
public:
  ~IStreamWriter() override = default;

  /** @brief Sets the buffers position relative to the specified position.<br />
   *         It seeks relative to the current position by default.
   *  @param position where in the buffer to seek
   *  @param origin The location to seek relative to
   */
  void seek(int64_t position, SeekOrigin origin = SeekOrigin::Current) override = 0;

  /** @brief Sets the buffers position relative to the next 32-byte aligned position.<br />
   */
  void seekAlign32() { seek(ROUND_UP_32(position()), SeekOrigin::Begin); }

  /** @brief Writes zero up to specified absolute offset.<br />
   */
  void writeZeroTo(int64_t pos) {
    int64_t delta = pos - position();
    if (delta <= 0)
      return;
    for (int64_t i = 0; i < delta; ++i)
      writeUByte(0);
  }

  /** @brief Returns whether or not the stream is at the end.
   *
   *  @return True if at end; False otherwise.
   */
  bool atEnd() const override { return position() >= length(); }

  /** @brief Returns the current position in the stream.
   *
   *  @return The current position in the stream.
   */
  uint64_t position() const override = 0;

  /** @brief Returns whether or not the stream is at the end.
   *
   *  @return True if at end; False otherwise.
   */
  uint64_t length() const override = 0;

  /** @brief Writes a byte at the current position and advances the position by one byte.
   *  @param val The value to write
   */
  void writeUByte(uint8_t val) { writeUBytes(&val, 1); }
  void writeVal(uint8_t val) { writeUByte(val); }
  void writeValLittle(uint8_t val) { writeUByte(val); }
  void writeValBig(uint8_t val) { writeUByte(val); }

  /** @brief Writes a byte at the current position and advances the position by one byte.
   *  @param val The value to write
   */
  void writeByte(int8_t val) { writeUByte(val); }
  void writeVal(int8_t val) { writeByte(val); }
  void writeValLittle(int8_t val) { writeByte(val); }
  void writeValBig(int8_t val) { writeByte(val); }

  /** @brief Writes the given buffer with the specified length, buffers can be bigger than the length
   *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
   *
   *  @param data The buffer to write
   *  @param length The amount to write
   */
  virtual void writeUBytes(const uint8_t* data, uint64_t length) = 0;

  /** @brief Writes the given buffer with the specified length, buffers can be bigger than the length
   *  however it's undefined behavior to try and write a buffer which is smaller than the given length.
   *
   *  @param data The buffer to write
   *  @param length The amount to write
   */
  void writeBytes(const void* data, uint64_t length) { writeUBytes(reinterpret_cast<const uint8_t*>(data), length); }

  /** @brief Writes an Int16 to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param val The value to write to the buffer
   */
  void writeInt16(int16_t val) {
    if (m_endian == Endian::Big) {
      utility::BigInt16(val);
    } else {
      utility::LittleInt16(val);
    }
    writeBytes(&val, sizeof(val));
  }
  void writeVal(int16_t val) { writeInt16(val); }

  /** @brief Writes an Int16 to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeInt16Little(int16_t val) {
    utility::LittleInt16(val);
    writeBytes(&val, sizeof(val));
  }
  void writeValLittle(int16_t val) { writeInt16Little(val); }

  /** @brief Writes an Int16 to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeInt16Big(int16_t val) {
    utility::BigInt16(val);
    writeBytes(&val, sizeof(val));
  }
  void writeValBig(int16_t val) { writeInt16Big(val); }

  /** @brief Writes an Uint16 to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings
   *
   *  @param val The value to write to the buffer
   */
  void writeUint16(uint16_t val) { writeInt16(val); }
  void writeVal(uint16_t val) { writeUint16(val); }

  /** @brief Writes an Uint16 to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform
   *
   *  @param val The value to write to the buffer
   */
  void writeUint16Little(uint16_t val) { writeInt16Little(val); }
  void writeValLittle(uint16_t val) { writeUint16Little(val); }

  /** @brief Writes an Uint16 to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform
   *
   *  @param val The value to write to the buffer
   */
  void writeUint16Big(uint16_t val) { writeInt16Big(val); }
  void writeValBig(uint16_t val) { writeUint16Big(val); }

  /** @brief Writes an Int32 to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param val The value to write to the buffer
   */
  void writeInt32(int32_t val) {
    if (m_endian == Endian::Big) {
      utility::BigInt32(val);
    } else {
      utility::LittleInt32(val);
    }
    writeBytes(&val, sizeof(val));
  }
  void writeVal(int32_t val) { writeInt32(val); }

  /** @brief Writes an Int32 to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeInt32Little(int32_t val) {
    utility::LittleInt32(val);
    writeBytes(&val, sizeof(val));
  }
  void writeValLittle(int32_t val) { writeInt32Little(val); }

  /** @brief Writes an Int32 to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeInt32Big(int32_t val) {
    utility::BigInt32(val);
    writeBytes(&val, sizeof(val));
  }
  void writeValBig(int32_t val) { writeInt32Big(val); }

  /** @brief Writes an Uint32 to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param val The value to write to the buffer
   */
  void writeUint32(uint32_t val) { writeInt32(val); }
  void writeVal(uint32_t val) { writeUint32(val); }

  /** @brief Writes an Uint32 to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeUint32Little(uint32_t val) { writeInt32Little(val); }
  void writeValLittle(uint32_t val) { writeUint32Little(val); }

  /** @brief Writes an Uint32 to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeUint32Big(uint32_t val) { writeInt32Big(val); }
  void writeValBig(uint32_t val) { writeUint32Big(val); }

  /** @brief Writes an Int64 to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param val The value to write to the buffer
   */
  void writeInt64(int64_t val) {
    if (m_endian == Endian::Big) {
      utility::BigInt64(val);
    } else {
      utility::LittleInt64(val);
    }
    writeBytes(&val, sizeof(val));
  }
  void writeVal(int64_t val) { writeInt64(val); }

  /** @brief Writes an Int64 to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeInt64Little(int64_t val) {
    utility::LittleInt64(val);
    writeBytes(&val, sizeof(val));
  }
  void writeValLittle(int64_t val) { writeInt64Little(val); }

  /** @brief Writes an Int64 to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeInt64Big(int64_t val) {
    utility::BigInt64(val);
    writeBytes(&val, sizeof(val));
  }
  void writeValBig(int64_t val) { writeInt64Big(val); }

  /** @brief Writes an Uint64 to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param val The value to write to the buffer
   */
  void writeUint64(uint64_t val) { writeInt64(val); }
  void writeVal(uint64_t val) { writeUint64(val); }

  /** @brief Writes an Uint64 to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeUint64Little(uint64_t val) { writeInt64Little(val); }
  void writeValLittle(uint64_t val) { writeUint64Little(val); }

  /** @brief Writes an Uint64 to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeUint64Big(uint64_t val) { writeInt64Big(val); }
  void writeValBig(uint64_t val) { writeUint64Big(val); }

  /** @brief Writes an float to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param val The value to write to the buffer
   */
  void writeFloat(float val) {
    if (m_endian == Endian::Big) {
      val = utility::BigFloat(val);
    } else {
      val = utility::LittleFloat(val);
    }
    writeBytes(&val, sizeof(val));
  }
  void writeVal(float val) { writeFloat(val); }

  /** @brief Writes an float to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeFloatLittle(float val) {
    utility::LittleFloat(val);
    writeBytes(&val, sizeof(val));
  }
  void writeValLittle(float val) { writeFloatLittle(val); }

  /** @brief Writes an float to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeFloatBig(float val) {
    val = utility::BigFloat(val);
    writeBytes(&val, sizeof(val));
  }
  void writeValBig(float val) { writeFloatBig(val); }

  /** @brief Writes an double to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param val The value to write to the buffer
   */
  void writeDouble(double val) {
    if (m_endian == Endian::Big) {
      utility::BigDouble(val);
    } else {
      utility::LittleDouble(val);
    }
    writeBytes(&val, sizeof(val));
  }
  void writeVal(double val) { writeDouble(val); }

  /** @brief Writes an double to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeDoubleLittle(double val) {
    utility::LittleDouble(val);
    writeBytes(&val, sizeof(val));
  }
  void writeValLittle(double val) { writeDoubleLittle(val); }

  /** @brief Writes an double to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param val The value to write to the buffer
   */
  void writeDoubleBig(double val) {
    utility::BigDouble(val);
    writeBytes(&val, sizeof(val));
  }
  void writeValBig(double val) { writeDoubleBig(val); }

  /** @brief Writes an bool to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param val The value to write to the buffer
   */
  void writeBool(bool val) {
    const auto u8Value = uint8_t(val);
    writeUBytes(&u8Value, sizeof(u8Value));
  }
  void writeVal(bool val) { writeBool(val); }
  void writeValLittle(bool val) { writeBool(val); }
  void writeValBig(bool val) { writeBool(val); }

  /** @brief Writes an atVec2f (8 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   * @param vec The value to write to the buffer
   */
  void writeVec2f(const atVec2f& vec) {
    simd_floats tmp(vec.simd);
    if (m_endian == Endian::Big) {
      tmp[0] = utility::BigFloat(tmp[0]);
      tmp[1] = utility::BigFloat(tmp[1]);
    } else {
      tmp[0] = utility::LittleFloat(tmp[0]);
      tmp[1] = utility::LittleFloat(tmp[1]);
    }
    writeBytes(tmp.data(), sizeof(simd_floats::value_type[2]));
  }
  void writeVal(const atVec2f& val) { writeVec2f(val); }

  /** @brief Writes an atVec2f (8 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   * @param vec The value to write to the buffer
   */
  void writeVec2fLittle(const atVec2f& vec) {
    simd_floats tmp(vec.simd);
    tmp[0] = utility::LittleFloat(tmp[0]);
    tmp[1] = utility::LittleFloat(tmp[1]);
    writeBytes(tmp.data(), sizeof(simd_floats::value_type[2]));
  }
  void writeValLittle(const atVec2f& val) { writeVec2fLittle(val); }

  /** @brief Writes an atVec2f (8 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   * @param vec The value to write to the buffer
   */
  void writeVec2fBig(const atVec2f& vec) {
    simd_floats tmp(vec.simd);
    tmp[0] = utility::BigFloat(tmp[0]);
    tmp[1] = utility::BigFloat(tmp[1]);
    writeBytes(tmp.data(), sizeof(simd_floats::value_type[2]));
  }
  void writeValBig(const atVec2f& val) { writeVec2fBig(val); }

  /** @brief Writes an atVec3f (12 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec3f(const atVec3f& vec) {
    simd_floats tmp(vec.simd);
    if (m_endian == Endian::Big) {
      tmp[0] = utility::BigFloat(tmp[0]);
      tmp[1] = utility::BigFloat(tmp[1]);
      tmp[2] = utility::BigFloat(tmp[2]);
    } else {
      tmp[0] = utility::LittleFloat(tmp[0]);
      tmp[1] = utility::LittleFloat(tmp[1]);
      tmp[2] = utility::LittleFloat(tmp[2]);
    }
    writeBytes(tmp.data(), sizeof(simd_floats::value_type[3]));
  }
  void writeVal(const atVec3f& val) { writeVec3f(val); }

  /** @brief Writes an atVec3f (12 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec3fLittle(const atVec3f& vec) {
    simd_floats tmp(vec.simd);
    tmp[0] = utility::LittleFloat(tmp[0]);
    tmp[1] = utility::LittleFloat(tmp[1]);
    tmp[2] = utility::LittleFloat(tmp[2]);
    writeBytes(tmp.data(), sizeof(simd_floats::value_type[3]));
  }
  void writeValLittle(const atVec3f& val) { writeVec3fLittle(val); }

  /** @brief Writes an atVec3f (12 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec3fBig(const atVec3f& vec) {
    simd_floats tmp(vec.simd);
    tmp[0] = utility::BigFloat(tmp[0]);
    tmp[1] = utility::BigFloat(tmp[1]);
    tmp[2] = utility::BigFloat(tmp[2]);
    writeBytes(tmp.data(), sizeof(simd_floats::value_type[3]));
  }
  void writeValBig(const atVec3f& val) { writeVec3fBig(val); }

  /** @brief Writes an atVec4f (16 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec4f(const atVec4f& vec) {
    simd_floats tmp(vec.simd);
    if (m_endian == Endian::Big) {
      tmp[0] = utility::BigFloat(tmp[0]);
      tmp[1] = utility::BigFloat(tmp[1]);
      tmp[2] = utility::BigFloat(tmp[2]);
      tmp[3] = utility::BigFloat(tmp[3]);
    } else {
      tmp[0] = utility::LittleFloat(tmp[0]);
      tmp[1] = utility::LittleFloat(tmp[1]);
      tmp[2] = utility::LittleFloat(tmp[2]);
      tmp[3] = utility::LittleFloat(tmp[3]);
    }
    writeBytes(tmp.data(), sizeof(simd_floats::value_type[4]));
  }
  void writeVal(const atVec4f& val) { writeVec4f(val); }

  /** @brief Writes an atVec4f (16 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec4fLittle(const atVec4f& vec) {
    simd_floats tmp(vec.simd);
    tmp[0] = utility::LittleFloat(tmp[0]);
    tmp[1] = utility::LittleFloat(tmp[1]);
    tmp[2] = utility::LittleFloat(tmp[2]);
    tmp[3] = utility::LittleFloat(tmp[3]);
    writeBytes(tmp.data(), sizeof(simd_floats::value_type[4]));
  }
  void writeValLittle(const atVec4f& val) { writeVec4fLittle(val); }

  /** @brief Writes an atVec4f (16 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec4fBig(const atVec4f& vec) {
    simd_floats tmp(vec.simd);
    tmp[0] = utility::BigFloat(tmp[0]);
    tmp[1] = utility::BigFloat(tmp[1]);
    tmp[2] = utility::BigFloat(tmp[2]);
    tmp[3] = utility::BigFloat(tmp[3]);
    writeBytes(tmp.data(), sizeof(simd_floats::value_type[4]));
  }
  void writeValBig(const atVec4f& val) { writeVec4fBig(val); }

  /** @brief Writes an atVec2d (16 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   * @param vec The value to write to the buffer
   */
  void writeVec2d(const atVec2d& vec) {
    simd_doubles tmp(vec.simd);
    if (m_endian == Endian::Big) {
      tmp[0] = utility::BigDouble(tmp[0]);
      tmp[1] = utility::BigDouble(tmp[1]);
    } else {
      tmp[0] = utility::LittleDouble(tmp[0]);
      tmp[1] = utility::LittleDouble(tmp[1]);
    }
    writeBytes(tmp.data(), sizeof(simd_doubles::value_type[2]));
  }
  void writeVal(const atVec2d& val) { writeVec2d(val); }

  /** @brief Writes an atVec2d (16 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   * @param vec The value to write to the buffer
   */
  void writeVec2dLittle(const atVec2d& vec) {
    simd_doubles tmp(vec.simd);
    tmp[0] = utility::LittleDouble(tmp[0]);
    tmp[1] = utility::LittleDouble(tmp[1]);
    writeBytes(tmp.data(), sizeof(simd_doubles::value_type[2]));
  }
  void writeValLittle(const atVec2d& val) { writeVec2dLittle(val); }

  /** @brief Writes an atVec2d (16 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   * @param vec The value to write to the buffer
   */
  void writeVec2dBig(const atVec2d& vec) {
    simd_doubles tmp(vec.simd);
    tmp[0] = utility::BigDouble(tmp[0]);
    tmp[1] = utility::BigDouble(tmp[1]);
    writeBytes(tmp.data(), sizeof(simd_doubles::value_type[2]));
  }
  void writeValBig(const atVec2d& val) { writeVec2dBig(val); }

  /** @brief Writes an atVec3d (24 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec3d(const atVec3d& vec) {
    simd_doubles tmp(vec.simd);
    if (m_endian == Endian::Big) {
      tmp[0] = utility::BigDouble(tmp[0]);
      tmp[1] = utility::BigDouble(tmp[1]);
      tmp[2] = utility::BigDouble(tmp[2]);
    } else {
      tmp[0] = utility::LittleDouble(tmp[0]);
      tmp[1] = utility::LittleDouble(tmp[1]);
      tmp[2] = utility::LittleDouble(tmp[2]);
    }
    writeBytes(tmp.data(), sizeof(simd_doubles::value_type[3]));
  }
  void writeVal(const atVec3d& val) { writeVec3d(val); }

  /** @brief Writes an atVec3d (24 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec3dLittle(const atVec3d& vec) {
    simd_doubles tmp(vec.simd);
    tmp[0] = utility::LittleDouble(tmp[0]);
    tmp[1] = utility::LittleDouble(tmp[1]);
    tmp[2] = utility::LittleDouble(tmp[2]);
    writeBytes(tmp.data(), sizeof(simd_doubles::value_type[3]));
  }
  void writeValLittle(const atVec3d& val) { writeVec3dLittle(val); }

  /** @brief Writes an atVec3d (24 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec3dBig(const atVec3d& vec) {
    simd_doubles tmp(vec.simd);
    tmp[0] = utility::BigDouble(tmp[0]);
    tmp[1] = utility::BigDouble(tmp[1]);
    tmp[2] = utility::BigDouble(tmp[2]);
    writeBytes(tmp.data(), sizeof(simd_doubles::value_type[3]));
  }
  void writeValBig(const atVec3d& val) { writeVec3dBig(val); }

  /** @brief Writes an atVec4d (32 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec4d(const atVec4d& vec) {
    simd_doubles tmp(vec.simd);
    if (m_endian == Endian::Big) {
      tmp[0] = utility::BigDouble(tmp[0]);
      tmp[1] = utility::BigDouble(tmp[1]);
      tmp[2] = utility::BigDouble(tmp[2]);
      tmp[3] = utility::BigDouble(tmp[3]);
    } else {
      tmp[0] = utility::LittleDouble(tmp[0]);
      tmp[1] = utility::LittleDouble(tmp[1]);
      tmp[2] = utility::LittleDouble(tmp[2]);
      tmp[3] = utility::LittleDouble(tmp[3]);
    }
    writeBytes(tmp.data(), sizeof(simd_doubles::value_type[4]));
  }
  void writeVal(const atVec4d& val) { writeVec4d(val); }

  /** @brief Writes an atVec4d (32 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against little depending on the platform.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec4dLittle(const atVec4d& vec) {
    simd_doubles tmp(vec.simd);
    tmp[0] = utility::LittleDouble(tmp[0]);
    tmp[1] = utility::LittleDouble(tmp[1]);
    tmp[2] = utility::LittleDouble(tmp[2]);
    tmp[3] = utility::LittleDouble(tmp[3]);
    writeBytes(tmp.data(), sizeof(simd_doubles::value_type[4]));
  }
  void writeValLittle(const atVec4d& val) { writeVec4dLittle(val); }

  /** @brief Writes an atVec4d (32 bytes) to the buffer and advances the buffer.
   *         It also swaps the bytes against big depending on the platform.
   *
   *  @param vec The value to write to the buffer
   */
  void writeVec4dBig(const atVec4d& vec) {
    simd_doubles tmp(vec.simd);
    tmp[0] = utility::BigDouble(tmp[0]);
    tmp[1] = utility::BigDouble(tmp[1]);
    tmp[2] = utility::BigDouble(tmp[2]);
    tmp[3] = utility::BigDouble(tmp[3]);
    writeBytes(tmp.data(), sizeof(simd_doubles::value_type[4]));
  }
  void writeValBig(const atVec4d& val) { writeVec4dBig(val); }

  /** @brief Converts a UTF8 string to a wide-char string in the buffer and advances the buffer.
   *         It also swaps the bytes depending on the platform and Stream settings.
   *
   *  @param str The string to write to the buffer
   *  @param fixedLen If not -1, the number of characters to zero-fill string to
   *
   *  Endianness is set with setEndian
   */
  void writeStringAsWString(std::string_view str, int32_t fixedLen = -1) {
    if (fixedLen == 0)
      return;
    std::string tmpStr = std::string("\xEF\xBB\xBF") + str.data();
    const utf8proc_uint8_t* buf = reinterpret_cast<const utf8proc_uint8_t*>(tmpStr.c_str());
    if (fixedLen < 0) {
      while (*buf) {
        utf8proc_int32_t wc;
        utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
        if (len < 0) {
          atWarning("invalid UTF-8 character while decoding");
          return;
        }
        buf += len;
        if (wc != 0xFEFF)
          writeUint16(uint16_t(wc));
      }
      writeUint16(0);
    } else {
      for (int32_t i = 0; i < fixedLen; ++i) {
        utf8proc_int32_t wc = 0;
        if (*buf) {
          utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
          if (len < 0) {
            atWarning("invalid UTF-8 character while decoding");
            return;
          }
          buf += len;
        }

        if (wc == 0xFEFF) {
          --i;
          continue;
        }

        writeUint16(uint16_t(wc));
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
  void writeStringAsWStringLittle(std::string_view str, int32_t fixedLen = -1) {
    if (fixedLen == 0)
      return;
    std::string tmpStr = std::string("\xEF\xBB\xBF") + str.data();
    const utf8proc_uint8_t* buf = reinterpret_cast<const utf8proc_uint8_t*>(tmpStr.c_str());
    if (fixedLen < 0) {
      while (*buf) {
        utf8proc_int32_t wc;
        utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
        if (len < 0) {
          atWarning("invalid UTF-8 character while decoding");
          return;
        }
        buf += len;
        if (wc != 0xFEFF)
          writeUint16Little(uint16_t(wc));
      }
      writeUint16Little(0);
    } else {
      for (int32_t i = 0; i < fixedLen; ++i) {
        utf8proc_int32_t wc = 0;
        if (*buf) {
          utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
          if (len < 0) {
            atWarning("invalid UTF-8 character while decoding");
            return;
          }
          buf += len;
        }

        if (wc == 0xFEFF) {
          --i;
          continue;
        }

        writeUint16Little(uint16_t(wc));
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
  void writeStringAsWStringBig(std::string_view str, int32_t fixedLen = -1) {
    if (fixedLen == 0)
      return;

    std::string tmpStr = std::string("\xEF\xBB\xBF") + str.data();
    const utf8proc_uint8_t* buf = reinterpret_cast<const utf8proc_uint8_t*>(tmpStr.c_str());
    if (fixedLen < 0) {
      while (*buf) {
        utf8proc_int32_t wc;
        utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
        if (len < 0) {
          atWarning("invalid UTF-8 character while decoding");
          return;
        }
        buf += len;
        if (wc != 0xFEFF)
          writeUint16Big(uint16_t(wc));
      }
      writeUint16Big(0);
    } else {
      for (int32_t i = 0; i < fixedLen; ++i) {
        utf8proc_int32_t wc = 0;
        if (*buf) {
          utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
          if (len < 0) {
            atWarning("invalid UTF-8 character while decoding");
            return;
          }
          buf += len;
        }

        if (wc == 0xFEFF) {
          --i;
          continue;
        }

        writeUint16Big(uint16_t(wc));
      }
    }
  }

  /** @brief Writes an string to the buffer and advances the buffer.
   *
   *  @param str The string to write to the buffer
   *  @param fixedLen If not -1, the number of characters to zero-fill string to
   */
  void writeString(std::string_view str, int32_t fixedLen = -1) {
    if (fixedLen == 0)
      return;

    if (fixedLen < 0) {
      for (uint8_t c : str) {
        writeUByte(c);

        if (c == '\0')
          break;
      }
      writeUByte(0);
    } else {
      auto it = str.begin();
      for (int32_t i = 0; i < fixedLen; ++i) {
        uint8_t chr;
        if (it == str.end())
          chr = 0;
        else
          chr = *it++;
        writeUByte(chr);
      }
    }
  }
  void writeVal(std::string_view val) { writeString(val); }

  /** @brief Writes an wstring to the buffer and advances the buffer.
   *
   *  @param str The string to write to the buffer
   *  @param fixedLen If not -1, the number of characters to zero-fill string to
   *
   *  Endianness is set with setEndian
   */
  void writeWString(std::wstring_view str, int32_t fixedLen = -1) {
    if (fixedLen == 0)
      return;

    if (fixedLen < 0) {
      for (uint16_t c : str) {
        writeUint16(c);

        if (c == L'\0')
          break;
      }
      writeUint16(0);
    } else {
      auto it = str.begin();
      for (int32_t i = 0; i < fixedLen; ++i) {
        uint16_t chr;
        if (it == str.end())
          chr = 0;
        else
          chr = *it++;
        writeUint16(chr);
      }
    }
  }
  void writeVal(std::wstring_view val) { writeWString(val); }

  /** @brief Writes an wstring to the buffer and advances the buffer.
   *
   *  @param str The string to write to the buffer
   *  @param fixedLen If not -1, the number of characters to zero-fill string to
   *
   *  Endianness is little
   */
  void writeWStringLittle(std::wstring_view str, int32_t fixedLen = -1) {
    if (fixedLen == 0)
      return;

    if (fixedLen < 0) {
      for (uint16_t c : str) {
        writeUint16Little(c);

        if (c == L'\0')
          break;
      }
      writeUint16Little(0);
    } else {
      auto it = str.begin();
      for (int32_t i = 0; i < fixedLen; ++i) {
        uint16_t chr;
        if (it == str.end())
          chr = 0;
        else
          chr = *it++;
        writeUint16Little(chr);
      }
    }
  }
  void writeValLittle(std::wstring_view val) { writeWStringLittle(val); }

  /** @brief Writes an wstring to the buffer and advances the buffer.
   *
   *  @param str The string to write to the buffer
   *  @param fixedLen If not -1, the number of characters to zero-fill string to
   *
   *  Endianness is big
   */
  void writeWStringBig(std::wstring_view str, int32_t fixedLen = -1) {
    if (fixedLen == 0)
      return;

    if (fixedLen < 0) {
      for (wchar_t c : str) {
        writeUint16Big(c);

        if (c == L'\0')
          break;
      }
      writeUint16Big(0);
    } else {
      auto it = str.begin();
      for (int32_t i = 0; i < fixedLen; ++i) {
        wchar_t chr;
        if (it == str.end())
          chr = 0;
        else
          chr = *it++;
        writeUint16Big(chr);
      }
    }
  }
  void writeValBig(std::wstring_view val) { writeWStringBig(val); }

  /** @brief Writes a u16string to the buffer and advances the buffer.
   *
   *  @param str The string to write to the buffer
   *  @param fixedLen If not -1, the number of characters to zero-fill string to
   *
   *  Endianness is big
   */
  void writeU16StringBig(std::u16string_view str, int32_t fixedLen = -1) {
    if (fixedLen == 0)
      return;

    if (fixedLen < 0) {
      for (char16_t c : str) {
        writeUint16Big(c);

        if (c == u'\0')
          break;
      }
      writeUint16Big(0);
    } else {
      auto it = str.begin();
      for (int32_t i = 0; i < fixedLen; ++i) {
        char16_t chr;
        if (it == str.end())
          chr = 0;
        else
          chr = *it++;
        writeUint16Big(chr);
      }
    }
  }
  void writeValBig(std::u16string_view val) { writeU16StringBig(val); }

  /** @brief Writes a u16string to the buffer and advances the buffer.
   *
   *  @param str The string to write to the buffer
   *  @param fixedLen If not -1, the number of characters to zero-fill string to
   *
   *  Endianness is big
   */
  void writeU32StringBig(std::u32string_view str, int32_t fixedLen = -1) {
    if (fixedLen == 0)
      return;

    if (fixedLen < 0) {
      for (char32_t c : str) {
        writeUint32Big(c);

        if (c == U'\0')
          break;
      }
      writeUint32Big(0);
    } else {
      auto it = str.begin();
      for (int32_t i = 0; i < fixedLen; ++i) {
        char32_t chr;
        if (it == str.end())
          chr = 0;
        else
          chr = *it++;
        writeUint32Big(chr);
      }
    }
  }
  void writeValBig(std::u32string_view val) { writeU32StringBig(val); }

  void fill(uint8_t val, uint64_t length) {
    if (length == 0)
      return;

    const std::vector<uint8_t> tmp(length, val);
    writeUBytes(tmp.data(), length);
  }

  void fill(int8_t val, uint64_t length) { fill((uint8_t)val, length); }

  /** @brief Performs automatic std::vector enumeration writes using numeric type T
   *  @param vector The std::vector read from when writing data
   *
   *  Endianness is set with setEndian
   */
  template <class T>
  void enumerate(const std::vector<T>& vector,
                 std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<T, atVec2f> || std::is_same_v<T, atVec3f> ||
                                  std::is_same_v<T, atVec4f>>* = nullptr) {
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
                       std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<T, atVec2f> ||
                                        std::is_same_v<T, atVec3f> || std::is_same_v<T, atVec4f>>* = nullptr) {
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
                    std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<T, atVec2f> ||
                                     std::is_same_v<T, atVec3f> || std::is_same_v<T, atVec4f>>* = nullptr) {
    for (const T& item : vector)
      writeValBig(item);
  }

  /** @brief Performs automatic std::vector enumeration writes using non-numeric type T
   *  @param vector The std::vector read from when writing data
   */
  template <class T>
  void enumerate(const std::vector<T>& vector,
                 std::enable_if_t<!std::is_arithmetic_v<T> && !std::is_same_v<T, atVec2f> &&
                                  !std::is_same_v<T, atVec3f> && !std::is_same_v<T, atVec4f>>* = nullptr) {
    for (const T& item : vector)
      item.write(*this);
  }
};

template <typename T>
IStreamWriter& operator<<(IStreamWriter& lhs, const T& rhs) {
  lhs.writeVal(rhs);
  return lhs;
}
} // namespace athena::io
