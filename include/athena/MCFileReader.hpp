#pragma once

#include "athena/MemoryReader.hpp"

namespace athena {

class MCFile;

namespace io {

/*! \class MCFileReader
 *  \brief The Minish Cap Save save data reader class
 *
 *  A Class for reading binary data from a The Minish Cap Save File,
 *  all work is done using a memory buffer, and not read directly from the disk.
 *  \sa BinaryReader
 */
class MCFileReader : public MemoryCopyReader {
public:
  /*!
   *  \brief This constructor takes an existing buffer to read from.
   *
   *  \param data The existing buffer
   *  \param length The length of the existing buffer
   */
  MCFileReader(uint8_t*, uint64_t);

  /*!
   *  \brief This constructor creates an instance from a file on disk.
   *
   *  \param filename The file to create the stream from
   */
  MCFileReader(const std::string&);

  /*!
   *  \brief Reads the save data from the buffer
   *
   *  \return MCFile* SRAM data
   */
  MCFile* readFile();
};

} // namespace io
} // namespace athena
