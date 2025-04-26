#pragma once

#include <string>
#include "athena/MemoryWriter.hpp"
#include "athena/ALTTPQuest.hpp"

namespace athena {
class ALTTPFile;

namespace io {
/*! \class ALTTPFileWriter
 *  \brief A Link to the Past save data writer class
 *
 *  A Class for writing binary data to an ALTTP Save File,
 *  all work is done using a memory buffer, and not written directly to the disk.
 *  \sa BinaryReader
 */
class ALTTPFileWriter : protected MemoryCopyWriter {
public:
  /*! \brief This constructor takes an existing buffer to write to.
   *
   *   \param data The existing buffer
   *   \param length The length of the existing buffer
   */
  ALTTPFileWriter(uint8_t*, uint64_t);

  /*! \brief This constructor creates an instance from a file on disk.
   *
   * \param filename The file to create the stream from
   */
  ALTTPFileWriter(const std::string&);

  /*! \brief Writes the given SRAM data to a file on disk
   *
   * \param file SRAM data to right
   */
  void writeFile(ALTTPFile* file);

private:
  void writeRoomFlags(ALTTPRoomFlags*);
  void writeOverworldEvent(ALTTPOverworldEvent*);
  void writeDungeonItems(ALTTPDungeonItemFlags);
  uint16_t calculateChecksum(uint32_t game);
};

} // namespace io
} // namespace athena
