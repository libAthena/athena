#pragma once

#include <functional>
#include <memory>
#include <type_traits>
#include <vector>

#include "athena/IStream.hpp"
#include "athena/Utility.hpp"

namespace athena::io {
/** @brief The IStreamReader class defines a basic API for reading from streams, Implementors are provided with one pure
 * virtual function that must be implemented in order to interact with the stream.
 *
 *  Most implementing classes will only need to implement IStreamReader::readUBytesToBuf(void*, atUint64) for basic
 * stream interaction
 */
class IStreamReader : public IStream {
public:
  ~IStreamReader() override = default;

  /** @brief Sets the buffers position relative to the specified position.<br />
   *         It seeks relative to the current position by default.
   *  @param position where in the buffer to seek
   *  @param origin The Origin to seek relative to
   */
  void seek(int64_t position, SeekOrigin origin = SeekOrigin::Current) override = 0;

  /** @brief Sets the buffer's position relative to the next 64-byte aligned position.<br />
   */
  void seekAlign64() { seek(ROUND_UP_64(position()), SeekOrigin::Begin); }

  /** @brief Sets the buffers position relative to the next 32-byte aligned position.<br />
   */
  void seekAlign32() { seek(ROUND_UP_32(position()), SeekOrigin::Begin); }

  /** @brief Sets the buffer's position relative to the next 16-byte aligned position.<br />
   */
  void seekAlign16() { seek(ROUND_UP_16(position()), SeekOrigin::Begin); }

  /** @brief Sets the buffer's position relative to the next 4-byte aligned position.<br />
   */
  void seekAlign4() { seek(ROUND_UP_4(position()), SeekOrigin::Begin); }

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

  /** @brief Returns the length of the file.
   *
   *  @return True length of the file.
   */
  uint64_t length() const override = 0;

  /** @brief Reads a byte at the current position and advances the current position
   *
   * @return The value at the current position
   */
  int8_t readByte() {
    int8_t val = 0;
    readUBytesToBuf(&val, 1);
    return val;
  }
  template <class T>
  int8_t readVal(std::enable_if_t<std::is_same_v<T, int8_t>>* = nullptr) {
    return readByte();
  }
  template <class T>
  int8_t readValLittle(std::enable_if_t<std::is_same_v<T, int8_t>>* = nullptr) {
    return readByte();
  }
  template <class T>
  int8_t readValBig(std::enable_if_t<std::is_same_v<T, int8_t>>* = nullptr) {
    return readByte();
  }

  /** @brief Reads a byte at the current position and advances the current position
   *
   * @return The value at the current position
   */
  uint8_t readUByte() { return readByte(); }
  template <class T>
  uint8_t readVal(std::enable_if_t<std::is_same_v<T, uint8_t>>* = nullptr) {
    return readUByte();
  }
  template <class T>
  uint8_t readValLittle(std::enable_if_t<std::is_same_v<T, uint8_t>>* = nullptr) {
    return readUByte();
  }
  template <class T>
  uint8_t readValBig(std::enable_if_t<std::is_same_v<T, uint8_t>>* = nullptr) {
    return readUByte();
  }

  /** @brief Reads a byte at the current position and advances the current position.
   *
   * @return The buffer at the current position from the given length.
   */
  std::unique_ptr<int8_t[]> readBytes(uint64_t length) {
    auto buf = std::make_unique<int8_t[]>(length);
    readUBytesToBuf(buf.get(), length);
    return buf;
  }

  /** @brief Reads a byte at the current position and advances the current position.
   *
   *  @return The buffer at the current position from the given length.
   */
  std::unique_ptr<uint8_t[]> readUBytes(uint64_t length) {
    auto buf = std::make_unique<uint8_t[]>(length);
    readUBytesToBuf(buf.get(), length);
    return buf;
  }

  /** @brief Attempts to read a fixed length of data into a pre-allocated buffer.
   *  @param buf The buffer to read into
   *  @param len The length of the buffer
   *  @return How much data was actually read, useful for detecting read errors.
   */
  uint64_t readBytesToBuf(void* buf, uint64_t len) { return readUBytesToBuf(buf, len); }

  /** @brief Attempts to read a fixed length of data into a pre-allocated buffer, this function is client defined
   *  and must be implemented.
   *  @param buf The buffer to read into
   *  @param len The length of the buffer
   *  @return How much data was actually read, useful for detecting read errors.
   */
  virtual uint64_t readUBytesToBuf(void* buf, uint64_t len) = 0;

  /** @brief Reads a Int16 and swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  int16_t readInt16() {
    int16_t val = 0;
    readUBytesToBuf(&val, 2);
    return m_endian == Endian::Big ? utility::BigInt16(val) : utility::LittleInt16(val);
  }
  template <class T>
  int16_t readVal(std::enable_if_t<std::is_same_v<T, int16_t>>* = nullptr) {
    return readInt16();
  }

  /** @brief Reads a Int16 and swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  int16_t readInt16Little() {
    int16_t val = 0;
    readUBytesToBuf(&val, 2);
    return utility::LittleInt16(val);
  }
  template <class T>
  int16_t readValLittle(std::enable_if_t<std::is_same_v<T, int16_t>>* = nullptr) {
    return readInt16Little();
  }

  /** @brief Reads a Int16 and swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  int16_t readInt16Big() {
    int16_t val = 0;
    readUBytesToBuf(&val, 2);
    return utility::BigInt16(val);
  }
  template <class T>
  int16_t readValBig(std::enable_if_t<std::is_same_v<T, int16_t>>* = nullptr) {
    return readInt16Big();
  }

  /** @brief Reads a Uint16 and swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  uint16_t readUint16() { return readInt16(); }
  template <class T>
  uint16_t readVal(std::enable_if_t<std::is_same_v<T, uint16_t>>* = 0) {
    return readUint16();
  }

  /** @brief Reads a Uint16 and swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  uint16_t readUint16Little() {
    uint16_t val = 0;
    readUBytesToBuf(&val, 2);
    return utility::LittleUint16(val);
  }
  template <class T>
  uint16_t readValLittle(std::enable_if_t<std::is_same_v<T, uint16_t>>* = nullptr) {
    return readUint16Little();
  }

  /** @brief Reads a Uint16 and swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  uint16_t readUint16Big() {
    uint16_t val = 0;
    readUBytesToBuf(&val, 2);
    return utility::BigUint16(val);
  }
  template <class T>
  uint16_t readValBig(std::enable_if_t<std::is_same_v<T, uint16_t>>* = nullptr) {
    return readUint16Big();
  }

  /** @brief Reads a Int32 and swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  int32_t readInt32() {
    int32_t val = 0;
    readUBytesToBuf(&val, 4);
    return m_endian == Endian::Big ? utility::BigInt32(val) : utility::LittleInt32(val);
  }
  template <class T>
  int32_t readVal(std::enable_if_t<std::is_same_v<T, int32_t>>* = nullptr) {
    return readInt32();
  }

  /** @brief Reads a Int32 and swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  int32_t readInt32Little() {
    int32_t val = 0;
    readUBytesToBuf(&val, 4);
    return utility::LittleInt32(val);
  }
  template <class T>
  int32_t readValLittle(std::enable_if_t<std::is_same_v<T, int32_t>>* = nullptr) {
    return readInt32Little();
  }

  /** @brief Reads a Int32 and swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  int32_t readInt32Big() {
    int32_t val = 0;
    readUBytesToBuf(&val, 4);
    return utility::BigInt32(val);
  }
  template <class T>
  int32_t readValBig(std::enable_if_t<std::is_same_v<T, int32_t>>* = nullptr) {
    return readInt32Big();
  }

  /** @brief Reads a Uint32 and swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  uint32_t readUint32() { return readInt32(); }
  template <class T>
  uint32_t readVal(std::enable_if_t<std::is_same_v<T, uint32_t>>* = nullptr) {
    return readUint32();
  }

  /** @brief Reads a Uint32 and swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  uint32_t readUint32Little() {
    uint32_t val = 0;
    readUBytesToBuf(&val, 4);
    return utility::LittleUint32(val);
  }
  template <class T>
  int32_t readValLittle(std::enable_if_t<std::is_same_v<T, uint32_t>>* = nullptr) {
    return readUint32Little();
  }

  /** @brief Reads a Uint32 and swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  uint32_t readUint32Big() {
    uint32_t val = 0;
    readUBytesToBuf(&val, 4);
    return utility::BigUint32(val);
  }
  template <class T>
  uint32_t readValBig(std::enable_if_t<std::is_same_v<T, uint32_t>>* = nullptr) {
    return readUint32Big();
  }

  /** @brief Reads a Int64 and swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  int64_t readInt64() {
    int64_t val = 0;
    readUBytesToBuf(&val, 8);
    return m_endian == Endian::Big ? utility::BigInt64(val) : utility::LittleInt64(val);
  }
  template <class T>
  int64_t readVal(std::enable_if_t<std::is_same_v<T, int64_t>>* = nullptr) {
    return readInt64();
  }

  /** @brief Reads a Int64 and swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  int64_t readInt64Little() {
    int64_t val = 0;
    readUBytesToBuf(&val, 8);
    return utility::LittleInt64(val);
  }
  template <class T>
  int64_t readValLittle(std::enable_if_t<std::is_same_v<T, int64_t>>* = nullptr) {
    return readInt64Little();
  }

  /** @brief Reads a Int64 and swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  int64_t readInt64Big() {
    int64_t val = 0;
    readUBytesToBuf(&val, 8);
    return utility::BigInt64(val);
  }
  template <class T>
  int64_t readValBig(std::enable_if_t<std::is_same_v<T, int64_t>>* = nullptr) {
    return readInt64Big();
  }

  /** @brief Reads a Uint64 and swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  uint64_t readUint64() { return readInt64(); }
  template <class T>
  uint64_t readVal(std::enable_if_t<std::is_same_v<T, uint64_t>>* = nullptr) {
    return readUint64();
  }

  /** @brief Reads a Uint64 and swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  uint64_t readUint64Little() {
    uint64_t val = 0;
    readUBytesToBuf(&val, 8);
    return utility::LittleUint64(val);
  }
  template <class T>
  uint64_t readValLittle(std::enable_if_t<std::is_same_v<T, uint64_t>>* = nullptr) {
    return readUint64Little();
  }

  /** @brief Reads a Uint64 and swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  uint64_t readUint64Big() {
    uint64_t val = 0;
    readUBytesToBuf(&val, 8);
    return utility::BigUint64(val);
  }
  template <class T>
  uint64_t readValBig(std::enable_if_t<std::is_same_v<T, uint64_t>>* = nullptr) {
    return readUint64Big();
  }

  /** @brief Reads a float and swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  float readFloat() {
    float val = 0.f;
    readUBytesToBuf(&val, 4);
    return m_endian == Endian::Big ? utility::BigFloat(val) : utility::LittleFloat(val);
  }
  template <class T>
  float readVal(std::enable_if_t<std::is_same_v<T, float>>* = nullptr) {
    return readFloat();
  }

  /** @brief Reads a float and swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  float readFloatLittle() {
    float val = 0.f;
    readUBytesToBuf(&val, 4);
    return utility::LittleFloat(val);
  }
  template <class T>
  float readValLittle(std::enable_if_t<std::is_same_v<T, float>>* = nullptr) {
    return readFloatLittle();
  }

  /** @brief Reads a float and swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  float readFloatBig() {
    float val = 0.f;
    readUBytesToBuf(&val, 4);
    return utility::BigFloat(val);
  }
  template <class T>
  float readValBig(std::enable_if_t<std::is_same_v<T, float>>* = nullptr) {
    return readFloatBig();
  }

  /** @brief Reads a double and swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  double readDouble() {
    double val = 0.0;
    readUBytesToBuf(&val, 8);
    return m_endian == Endian::Big ? utility::BigDouble(val) : utility::LittleDouble(val);
  }
  template <class T>
  double readVal(std::enable_if_t<std::is_same_v<T, double>>* = nullptr) {
    return readDouble();
  }

  /** @brief Reads a double and swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  double readDoubleLittle() {
    double val = 0.0;
    readUBytesToBuf(&val, 8);
    return utility::LittleDouble(val);
  }
  template <class T>
  double readValLittle(std::enable_if_t<std::is_same_v<T, double>>* = nullptr) {
    return readDoubleLittle();
  }

  /** @brief Reads a double and swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  double readDoubleBig() {
    double val = 0.0;
    readUBytesToBuf(&val, 8);
    return utility::BigDouble(val);
  }
  template <class T>
  double readValBig(std::enable_if_t<std::is_same_v<T, double>>* = nullptr) {
    return readDoubleBig();
  }

  /** @brief Reads a bool and advances the current position
   *
   *  @return The value at the current address
   */
  bool readBool() {
    uint8_t val = false;
    readUBytesToBuf(&val, 1);
    return val != 0;
  }
  template <class T>
  bool readVal(std::enable_if_t<std::is_same_v<T, bool>>* = nullptr) {
    return readBool();
  }
  template <class T>
  bool readValLittle(std::enable_if_t<std::is_same_v<T, bool>>* = nullptr) {
    return readBool();
  }
  template <class T>
  bool readValBig(std::enable_if_t<std::is_same_v<T, bool>>* = nullptr) {
    return readBool();
  }

  /** @brief Reads an atVec2f (8 bytes), swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec2f readVec2f() {
    simd_floats val = {};
    readUBytesToBuf(val.data(), 8);
    if (m_endian == Endian::Big) {
      val[0] = utility::BigFloat(val[0]);
      val[1] = utility::BigFloat(val[1]);
    } else {
      val[0] = utility::LittleFloat(val[0]);
      val[1] = utility::LittleFloat(val[1]);
    }
    val[2] = 0.f;
    val[3] = 0.f;
    atVec2f s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec2f readVal(std::enable_if_t<std::is_same_v<T, atVec2f>>* = nullptr) {
    return readVec2f();
  }

  /** @brief Reads an atVec2f (8 bytes), swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec2f readVec2fLittle() {
    simd_floats val = {};
    readUBytesToBuf(val.data(), 8);
    val[0] = utility::LittleFloat(val[0]);
    val[1] = utility::LittleFloat(val[1]);
    val[2] = 0.f;
    val[3] = 0.f;
    atVec2f s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec2f readValLittle(std::enable_if_t<std::is_same_v<T, atVec2f>>* = nullptr) {
    return readVec2fLittle();
  }

  /** @brief Reads an atVec2f (8 bytes), swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec2f readVec2fBig() {
    simd_floats val = {};
    readUBytesToBuf(val.data(), 8);
    val[0] = utility::BigFloat(val[0]);
    val[1] = utility::BigFloat(val[1]);
    val[2] = 0.f;
    val[3] = 0.f;
    atVec2f s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec2f readValBig(std::enable_if_t<std::is_same_v<T, atVec2f>>* = nullptr) {
    return readVec2fBig();
  }

  /** @brief Reads an atVec3f (12 bytes), swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec3f readVec3f() {
    simd_floats val = {};
    readUBytesToBuf(val.data(), 12);
    if (m_endian == Endian::Big) {
      val[0] = utility::BigFloat(val[0]);
      val[1] = utility::BigFloat(val[1]);
      val[2] = utility::BigFloat(val[2]);
    } else {
      val[0] = utility::LittleFloat(val[0]);
      val[1] = utility::LittleFloat(val[1]);
      val[2] = utility::LittleFloat(val[2]);
    }
    val[3] = 0.f;
    atVec3f s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec3f readVal(std::enable_if_t<std::is_same_v<T, atVec3f>>* = nullptr) {
    return readVec3f();
  }

  /** @brief Reads an atVec3f (12 bytes), swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec3f readVec3fLittle() {
    simd_floats val = {};
    readUBytesToBuf(val.data(), 12);
    val[0] = utility::LittleFloat(val[0]);
    val[1] = utility::LittleFloat(val[1]);
    val[2] = utility::LittleFloat(val[2]);
    val[3] = 0.f;
    atVec3f s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec3f readValLittle(std::enable_if_t<std::is_same_v<T, atVec3f>>* = nullptr) {
    return readVec3fLittle();
  }

  /** @brief Reads an atVec3f (12 bytes), swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec3f readVec3fBig() {
    simd_floats val = {};
    readUBytesToBuf(val.data(), 12);
    val[0] = utility::BigFloat(val[0]);
    val[1] = utility::BigFloat(val[1]);
    val[2] = utility::BigFloat(val[2]);
    val[3] = 0.f;
    atVec3f s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec3f readValBig(std::enable_if_t<std::is_same_v<T, atVec3f>>* = nullptr) {
    return readVec3fBig();
  }

  /** @brief Reads an atVec4f (16 bytes), swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec4f readVec4f() {
    simd_floats val = {};
    readUBytesToBuf(val.data(), 16);
    if (m_endian == Endian::Big) {
      val[0] = utility::BigFloat(val[0]);
      val[1] = utility::BigFloat(val[1]);
      val[2] = utility::BigFloat(val[2]);
      val[3] = utility::BigFloat(val[3]);
    } else {
      val[0] = utility::LittleFloat(val[0]);
      val[1] = utility::LittleFloat(val[1]);
      val[2] = utility::LittleFloat(val[2]);
      val[3] = utility::LittleFloat(val[3]);
    }
    atVec4f s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec4f readVal(std::enable_if_t<std::is_same_v<T, atVec4f>>* = nullptr) {
    return readVec4f();
  }

  /** @brief Reads an atVec4f (16 bytes), swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec4f readVec4fLittle() {
    simd_floats val = {};
    readUBytesToBuf(val.data(), 16);
    val[0] = utility::LittleFloat(val[0]);
    val[1] = utility::LittleFloat(val[1]);
    val[2] = utility::LittleFloat(val[2]);
    val[3] = utility::LittleFloat(val[3]);
    atVec4f s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec4f readValLittle(std::enable_if_t<std::is_same_v<T, atVec4f>>* = nullptr) {
    return readVec4fLittle();
  }

  /** @brief Reads an atVec4f (16 bytes), swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec4f readVec4fBig() {
    simd_floats val = {};
    readUBytesToBuf(val.data(), 16);
    val[0] = utility::BigFloat(val[0]);
    val[1] = utility::BigFloat(val[1]);
    val[2] = utility::BigFloat(val[2]);
    val[3] = utility::BigFloat(val[3]);
    atVec4f s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec4f readValBig(std::enable_if_t<std::is_same_v<T, atVec4f>>* = nullptr) {
    return readVec4fBig();
  }

  /** @brief Reads an atVec2d (16 bytes), swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec2d readVec2d() {
    simd_doubles val = {};
    readUBytesToBuf(val.data(), 16);
    if (m_endian == Endian::Big) {
      val[0] = utility::BigDouble(val[0]);
      val[1] = utility::BigDouble(val[1]);
    } else {
      val[0] = utility::LittleDouble(val[0]);
      val[1] = utility::LittleDouble(val[1]);
    }
    val[2] = 0.0;
    val[3] = 0.0;
    atVec2d s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec2d readVal(std::enable_if_t<std::is_same_v<T, atVec2d>>* = nullptr) {
    return readVec2d();
  }

  /** @brief Reads an atVec2d (16 bytes), swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec2d readVec2dLittle() {
    simd_doubles val = {};
    readUBytesToBuf(val.data(), 16);
    val[0] = utility::LittleDouble(val[0]);
    val[1] = utility::LittleDouble(val[1]);
    val[2] = 0.0;
    val[3] = 0.0;
    atVec2d s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec2d readValLittle(std::enable_if_t<std::is_same_v<T, atVec2d>>* = nullptr) {
    return readVec2dLittle();
  }

  /** @brief Reads an atVec2d (16 bytes), swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec2d readVec2dBig() {
    simd_doubles val = {};
    readUBytesToBuf(val.data(), 16);
    val[0] = utility::BigDouble(val[0]);
    val[1] = utility::BigDouble(val[1]);
    val[2] = 0.0;
    val[3] = 0.0;
    atVec2d s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec2d readValBig(std::enable_if_t<std::is_same_v<T, atVec2d>>* = nullptr) {
    return readVec2dBig();
  }

  /** @brief Reads an atVec3d (24 bytes), swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec3d readVec3d() {
    simd_doubles val = {};
    readUBytesToBuf(val.data(), 24);
    if (m_endian == Endian::Big) {
      val[0] = utility::BigDouble(val[0]);
      val[1] = utility::BigDouble(val[1]);
      val[2] = utility::BigDouble(val[2]);
    } else {
      val[0] = utility::LittleDouble(val[0]);
      val[1] = utility::LittleDouble(val[1]);
      val[2] = utility::LittleDouble(val[2]);
    }
    val[3] = 0.0;
    atVec3d s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec3d readVal(std::enable_if_t<std::is_same_v<T, atVec3d>>* = nullptr) {
    return readVec3d();
  }

  /** @brief Reads an atVec3d (24 bytes), swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec3d readVec3dLittle() {
    simd_doubles val = {};
    readUBytesToBuf(val.data(), 24);
    val[0] = utility::LittleDouble(val[0]);
    val[1] = utility::LittleDouble(val[1]);
    val[2] = utility::LittleDouble(val[2]);
    val[3] = 0.0;
    atVec3d s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec3d readValLittle(std::enable_if_t<std::is_same_v<T, atVec3d>>* = nullptr) {
    return readVec3dLittle();
  }

  /** @brief Reads an atVec3d (24 bytes), swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec3d readVec3dBig() {
    simd_doubles val = {};
    readUBytesToBuf(val.data(), 24);
    val[0] = utility::BigDouble(val[0]);
    val[1] = utility::BigDouble(val[1]);
    val[2] = utility::BigDouble(val[2]);
    val[3] = 0.0;
    atVec3d s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec3d readValBig(std::enable_if_t<std::is_same_v<T, atVec3d>>* = nullptr) {
    return readVec3dBig();
  }

  /** @brief Reads an atVec4d (32 bytes), swaps to endianness specified by setEndian depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec4d readVec4d() {
    simd_doubles val = {};
    readUBytesToBuf(val.data(), 32);
    if (m_endian == Endian::Big) {
      val[0] = utility::BigDouble(val[0]);
      val[1] = utility::BigDouble(val[1]);
      val[2] = utility::BigDouble(val[2]);
      val[3] = utility::BigDouble(val[3]);
    } else {
      val[0] = utility::LittleDouble(val[0]);
      val[1] = utility::LittleDouble(val[1]);
      val[2] = utility::LittleDouble(val[2]);
      val[3] = utility::LittleDouble(val[3]);
    }
    atVec4d s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec4d readVal(std::enable_if_t<std::is_same_v<T, atVec4d>>* = nullptr) {
    return readVec4d();
  }

  /** @brief Reads an atVec4d (32 bytes), swaps against little endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec4d readVec4dLittle() {
    simd_doubles val = {};
    readUBytesToBuf(val.data(), 32);
    val[0] = utility::LittleDouble(val[0]);
    val[1] = utility::LittleDouble(val[1]);
    val[2] = utility::LittleDouble(val[2]);
    val[3] = utility::LittleDouble(val[3]);
    atVec4d s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec4d readValLittle(std::enable_if_t<std::is_same_v<T, atVec4d>>* = nullptr) {
    return readVec4dLittle();
  }

  /** @brief Reads an atVec4d (32 bytes), swaps against big endianness depending on platform
   *  and advances the current position
   *
   *  @return The value at the current address
   */
  atVec4d readVec4dBig() {
    simd_doubles val = {};
    readUBytesToBuf(val.data(), 32);
    val[0] = utility::BigDouble(val[0]);
    val[1] = utility::BigDouble(val[1]);
    val[2] = utility::BigDouble(val[2]);
    val[3] = utility::BigDouble(val[3]);
    atVec4d s;
    s.simd.copy_from(val);
    return s;
  }
  template <class T>
  atVec4d readValBig(std::enable_if_t<std::is_same_v<T, atVec4d>>* = nullptr) {
    return readVec4dBig();
  }

  /** @brief Reads a string and advances the position in the file
   *
   *  @param fixedLen If non-negative, this is a fixed-length string read.
   *  @param doSeek   Whether or not to reset the advanced position of the file.
   *                  This is ignored if fixedLen is less than or equal to zero.
   *
   *  @return The read string
   */
  std::string readString(int32_t fixedLen = -1, bool doSeek = true) {
    if (fixedLen == 0)
      return std::string();
    std::string ret;
    uint8_t chr = readByte();

    int32_t i;
    for (i = 1; chr != 0; ++i) {
      ret += chr;

      if (fixedLen > 0 && i >= fixedLen)
        break;

      chr = readByte();
    }

    if (doSeek && fixedLen > 0 && i < fixedLen)
      seek(fixedLen - i);

    return ret;
  }
  template <class T>
  std::string readVal(std::enable_if_t<std::is_same_v<T, std::string>>* = nullptr) {
    return readString();
  }

  /** @brief Reads a wstring and advances the position in the file
   *
   *  @param fixedLen If non-negative, this is a fixed-length string read.
   *  @param doSeek   Whether or not to reset the advanced position of the file.
   *                  This is ignored if fixedLen is less than or equal to zero.
   *
   *  @return The read wstring
   */
  std::wstring readWString(int32_t fixedLen = -1, bool doSeek = true) {
    if (fixedLen == 0)
      return std::wstring();

    std::wstring ret;
    uint16_t chr = readUint16();

    int32_t i;
    for (i = 1; chr != 0; ++i) {
      ret += chr;

      if (fixedLen > 0 && i >= fixedLen)
        break;

      chr = readUint16();
    }

    if (doSeek && fixedLen > 0 && i < fixedLen)
      seek(fixedLen - i);

    return ret;
  }
  template <class T>
  std::wstring readVal(std::enable_if_t<std::is_same_v<T, std::wstring>>* = nullptr) {
    return readWString();
  }

  /** @brief Reads a wstring assuming little-endian characters
   *         and advances the position in the file
   *
   *  @param fixedLen If non-negative, this is a fixed-length string read.
   *  @param doSeek   Whether or not to reset the advanced position of the file.
   *                  This is ignored if fixedLen is less than or equal to zero.
   *
   *  @return The read wstring
   */
  std::wstring readWStringLittle(int32_t fixedLen = -1, bool doSeek = true) {
    if (fixedLen == 0)
      return std::wstring();

    std::wstring ret;
    uint16_t chr = readUint16Little();

    int32_t i;
    for (i = 1; chr != 0; ++i) {
      ret += chr;

      if (fixedLen > 0 && i >= fixedLen)
        break;

      chr = readUint16Little();
    }

    if (doSeek && fixedLen > 0 && i < fixedLen)
      seek(fixedLen - i);

    return ret;
  }
  template <class T>
  std::wstring readValLittle(std::enable_if_t<std::is_same_v<T, std::wstring>>* = nullptr) {
    return readWStringLittle();
  }

  /** @brief Reads a wstring assuming big-endian characters
   *         and advances the position in the file
   *
   *  @param fixedLen If non-negative, this is a fixed-length string read
   *  @param doSeek   Whether or not to reset the advanced position of the file.
   *                  This is ignored if fixedLen is less than or equal to zero.
   *
   *  @return The read wstring
   */
  std::wstring readWStringBig(int32_t fixedLen = -1, bool doSeek = true) {
    if (fixedLen == 0)
      return std::wstring();
    std::wstring ret;
    uint16_t chr = readUint16Big();

    int32_t i;
    for (i = 1; chr != 0; ++i) {
      ret += chr;

      if (fixedLen > 0 && i >= fixedLen)
        break;

      chr = readUint16Big();
    }

    if (doSeek && fixedLen > 0 && i < fixedLen)
      seek(fixedLen - i);

    return ret;
  }
  template <class T>
  std::wstring readValBig(std::enable_if_t<std::is_same_v<T, std::wstring>>* = nullptr) {
    return readWStringBig();
  }

  /** @brief Reads a u16string assuming big-endian characters
   *         and advances the position in the file
   *
   *  @param fixedLen If non-negative, this is a fixed-length string read.
   *  @param doSeek   Whether or not to reset the advanced position of the file.
   *                  This is ignored if fixedLen is less than or equal to zero.
   *
   *  @return The read wstring
   */
  std::u16string readU16StringBig(int32_t fixedLen = -1, bool doSeek = true) {
    if (fixedLen == 0)
      return std::u16string();
    std::u16string ret;
    char16_t chr = readUint16Big();

    int32_t i;
    for (i = 1; chr != 0; ++i) {
      ret += chr;

      if (fixedLen > 0 && i >= fixedLen)
        break;

      chr = readUint16Big();
    }

    if (doSeek && fixedLen > 0 && i < fixedLen)
      seek(fixedLen - i);

    return ret;
  }
  template <class T>
  std::u16string readValBig(std::enable_if_t<std::is_same_v<T, std::u16string>>* = nullptr) {
    return readU16StringBig();
  }

  /** @brief Reads a u32string assuming big-endian characters
   *         and advances the position in the file
   *
   *  @param fixedLen If non-negative, this is a fixed-length string read.
   *  @param doSeek   Whether or not to reset the advanced position of the file.
   *                  This is ignored if fixedLen is less than or equal to zero.
   *
   *  @return The read wstring
   */
  std::u32string readU32StringBig(int32_t fixedLen = -1, bool doSeek = true) {
    if (fixedLen == 0)
      return std::u32string();
    std::u32string ret;
    char32_t chr = readUint32Big();

    int32_t i;
    for (i = 1; chr != 0; ++i) {
      ret += chr;

      if (fixedLen > 0 && i >= fixedLen)
        break;

      chr = readUint32Big();
    }

    if (doSeek && fixedLen > 0 && i < fixedLen)
      seek(fixedLen - i);

    return ret;
  }
  template <class T>
  std::u32string readValBig(std::enable_if_t<std::is_same_v<T, std::u32string>>* = nullptr) {
    return readU32StringBig();
  }

  /** @brief Performs automatic std::vector enumeration reads using numeric type T
   *
   *  @param vector The std::vector to clear and populate using read data
   *  @param count The number of elements to read into vector
   *
   *  Endianness is set with setEndian
   */
  template <class T>
  void enumerate(std::vector<T>& vector, size_t count,
                 std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<T, atVec2f> || std::is_same_v<T, atVec3f> ||
                                  std::is_same_v<T, atVec4f>>* = nullptr) {
    vector.clear();
    vector.reserve(count);
    for (size_t i = 0; i < count; ++i)
      vector.push_back(readVal<T>());
  }

  /** @brief Performs automatic std::vector enumeration reads using numeric type T
   *
   *  @param vector The std::vector to clear and populate using read data
   *  @param count The number of elements to read into vector
   *
   *  Endianness is little
   */
  template <class T>
  void enumerateLittle(std::vector<T>& vector, size_t count,
                       std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<T, atVec2f> ||
                                        std::is_same_v<T, atVec3f> || std::is_same_v<T, atVec4f>>* = nullptr) {
    vector.clear();
    vector.reserve(count);
    for (size_t i = 0; i < count; ++i)
      vector.push_back(readValLittle<T>());
  }

  /** @brief Performs automatic std::vector enumeration reads using numeric type T
   *
   *  @param vector The std::vector to clear and populate using read data
   *  @param count The number of elements to read into vector
   *
   *  Endianness is big
   */
  template <class T>
  void enumerateBig(std::vector<T>& vector, size_t count,
                    std::enable_if_t<std::is_arithmetic_v<T> || std::is_same_v<T, atVec2f> ||
                                     std::is_same_v<T, atVec3f> || std::is_same_v<T, atVec4f>>* = nullptr) {
    vector.clear();
    vector.reserve(count);
    for (size_t i = 0; i < count; ++i)
      vector.push_back(readValBig<T>());
  }

  /** @brief Performs automatic std::vector enumeration reads using non-numeric type T
   *
   *  @param vector The std::vector to clear and populate using read data
   *  @param count The number of elements to read into vector
   */
  template <class T>
  void enumerate(std::vector<T>& vector, size_t count,
                 std::enable_if_t<!std::is_arithmetic_v<T> && !std::is_same_v<T, atVec2f> &&
                                  !std::is_same_v<T, atVec3f> && !std::is_same_v<T, atVec4f>>* = nullptr) {
    vector.clear();
    vector.reserve(count);
    for (size_t i = 0; i < count; ++i) {
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
  template <class T>
  void enumerate(std::vector<T>& vector, size_t count, std::function<void(IStreamReader&, T&)> readf) {
    vector.clear();
    vector.reserve(count);
    for (size_t i = 0; i < count; ++i) {
      vector.emplace_back();
      readf(*this, vector.back());
    }
  }
};
template <typename T>
IStreamReader& operator>>(IStreamReader& lhs, T& rhs) {
  rhs = lhs.readVal<T>();
  return lhs;
}
} // namespace athena::io
