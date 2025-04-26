#pragma once

#include "athena/MemoryWriter.hpp"

namespace athena {
class WiiSave;
class WiiBanner;
class WiiFile;
class WiiImage;

namespace io {

/*! \class WiiSaveWriter
 *  \brief Wii data.bin writer class
 *
 *  A Class for writing binary data to a wii data.bin file,
 *  all work is done using a memory buffer, and not written directly to the disk.
 *  \sa BinaryReader
 */
class WiiSaveWriter : protected MemoryCopyWriter {
public:
  /*! \brief This constructor creates an instance from a file on disk.
   *
   * \param filename The file to create the stream from
   */
  WiiSaveWriter(const std::string&);

  /*!
   * \brief writeSave
   * \param save
   * \param macAddress
   * \param ngId
   * \param ngPriv
   * \param ngSig
   * \param ngKeyId
   * \param filepath
   * \return
   */
  bool writeSave(WiiSave* save, uint8_t* macAddress, uint32_t ngId, uint8_t* ngPriv, uint8_t* ngSig, uint32_t ngKeyId,
                 const std::string& filepath = "");

private:
  void writeBanner(WiiBanner* banner);
  uint32_t writeFile(WiiFile* file);
  void writeImage(WiiImage* image);
  void writeCerts(uint32_t filesSize, uint32_t ngId, uint8_t* ngPriv, uint8_t* ngSig, uint32_t ngKeyId);
};

} // namespace io
} // namespace athena
