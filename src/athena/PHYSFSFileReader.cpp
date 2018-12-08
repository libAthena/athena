#ifdef ATHENA_ENABLE_PHYSFS
#include "athena/PHYSFSFileReader.hpp"
#include "athena/FileNotFoundException.hpp"
#include "athena/IOException.hpp"

namespace athena::io {

PHYSFSFileReader::PHYSFSFileReader(const std::string& path) : m_path(path), m_endian(Endian::LittleEndian) {
  if (!PHYSFS_exists(m_path.c_str()))
    THROW_FILE_NOT_FOUND_EXCEPTION(path);

  m_handle = PHYSFS_openRead(path.c_str());
  m_length = PHYSFS_fileLength(m_handle);
}

atUint8* PHYSFSFileReader::data() { return readUBytes(length()); }

atUint64 PHYSFSFileReader::length() const { return m_length; }

atUint64 PHYSFSFileReader::position() const { return PHYSFS_tell(m_handle); }

bool PHYSFSFileReader::isOpen() const { return (m_handle != nullptr); }

void PHYSFSFileReader::seek(atInt64 position, SeekOrigin origin) {
  atInt64 curPos = PHYSFS_tell(m_handle);

  switch (origin) {
  case SeekOrigin::Begin:
    if ((position < 0 || (atInt64)position > (atInt64)m_length))
      THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", position);

    PHYSFS_seek(m_handle, position);
    break;

  case SeekOrigin::Current:
    if ((((atInt64)curPos + position) < 0 || (curPos + position) > m_length))
      THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", position);

    PHYSFS_seek(m_handle, curPos + position);
    break;

  case SeekOrigin::End:
    if ((((atInt64)m_length - position < 0) || (m_length - position) > m_length))
      THROW_IO_EXCEPTION("Position %0.16X outside stream bounds ", position);

    PHYSFS_seek(m_handle, m_length - position);
    break;
  }
}

atUint8 PHYSFSFileReader::readUByte() {
  atUint8 val;

  if (!PHYSFS_read(m_handle, &val, 1, 1))
    THROW_IO_EXCEPTION("Position outside stream bounds");

  return val;
}

atInt8 PHYSFSFileReader::readByte() { return readUByte(); }

atUint8* PHYSFSFileReader::readUBytes(atUint64 length) {
  atUint8* data = new atUint8[length];

  if (PHYSFS_read(m_handle, data, 1, length) == length)
    return data;

  delete[] data;
  THROW_IO_EXCEPTION("Position outside stream bounds");
}

atInt8* PHYSFSFileReader::readBytes(atUint64 length) { return (atInt8*)readUBytes(length); }

atUint16 PHYSFSFileReader::readUint16() {
  atUint16 val;

  if (m_endian == Endian::BigEndian) {
    if (!PHYSFS_readUBE16(m_handle, &val))
      THROW_IO_EXCEPTION("Position outside stream bounds");
  } else if (!PHYSFS_readULE16(m_handle, &val))
    THROW_IO_EXCEPTION("Position outside stream bounds");

  return val;
}

atInt16 PHYSFSFileReader::readInt16() { return readUint16(); }

atUint32 PHYSFSFileReader::readUint32() {
  atUint32 val;

  if (m_endian == Endian::BigEndian) {
    if (!PHYSFS_readUBE32(m_handle, &val))
      THROW_IO_EXCEPTION("Position outside stream bounds");
  } else if (!PHYSFS_readULE32(m_handle, &val))
    THROW_IO_EXCEPTION("Position outside stream bounds");

  return val;
}

atInt32 PHYSFSFileReader::readInt32() { return readUint32(); }

atUint64 PHYSFSFileReader::readUint64() {
  atUint64 val;

  if (m_endian == Endian::BigEndian) {
    if (!PHYSFS_readUBE64(m_handle, &val))
      THROW_IO_EXCEPTION("Position outside stream bounds");
  } else if (!PHYSFS_readUBE64(m_handle, &val))
    THROW_IO_EXCEPTION("Position outside stream bounds");

  return val;
}

atInt64 PHYSFSFileReader::readInt64() { return readUint64(); }

double PHYSFSFileReader::readDouble() { return static_cast<double>(readUint64()); }

float PHYSFSFileReader::readFloat() { return static_cast<float>(readUint32()); }

bool PHYSFSFileReader::readBool() { return (readByte() != 0); }

bool PHYSFSFileReader::atEnd() const { return PHYSFS_eof(m_handle); }

void PHYSFSFileReader::setEndian(Endian endian) { m_endian = endian; }

Endian PHYSFSFileReader::endian() const { return m_endian; }

bool PHYSFSFileReader::isBigEndian() const { return m_endian == Endian::BigEndian; }

bool PHYSFSFileReader::isLittleEndian() const { return m_endian == Endian::LittleEndian; }

} // namespace athena::io

#endif
