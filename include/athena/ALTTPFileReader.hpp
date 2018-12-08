#pragma once

#include <string>
#include "athena/Types.hpp"
#include "athena/MemoryReader.hpp"
#include "athena/ALTTPQuest.hpp"

namespace athena {
class ALTTPFile;

namespace io {
/*! \class ALTTPFileReader
 *  \brief A Link to the Past save data reader class
 *
 *  A Class for reading binary data from an ALTTP Save File,
 *  all work is done using a memory buffer, and not read directly from the disk.
 *  \sa BinaryReader
 */
class ALTTPFileReader : protected MemoryCopyReader {
public:
  /*! \brief This constructor takes an existing buffer to read from.
   *
   *   \param data The existing buffer
   *   \param length The length of the existing buffer
   */
  ALTTPFileReader(atUint8*, atUint64);

  /*! \brief This constructor creates an instance from a file on disk.
   *
   * \param filename The file to create the stream from
   */
  ALTTPFileReader(const std::string&);

  /*! \brief Reads the SRAM data from the buffer
   *
   * \return ALTTPFile* SRAM data
   */
  ALTTPFile* readFile();

private:
  ALTTPRoomFlags* readRoomFlags();
  ALTTPOverworldEvent* readOverworldEvent();
  ALTTPDungeonItemFlags readDungeonFlags();
};

} // namespace io
} // namespace athena
