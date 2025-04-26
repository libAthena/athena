#ifndef ATHENA_NO_ZQUEST
#ifndef __ZQUESTFILEREADER_HPP__
#define __ZQUESTFILEREADER_HPP__

#include "athena/MemoryReader.hpp"

namespace athena {
class ZQuestFile;

namespace io {

/*!
 * \brief The ZQuestFileReader class
 */
class ZQuestFileReader : protected MemoryCopyReader {

public:
  /*!
   * \brief ZQuestFileReader
   * \param data
   * \param length
   */
  ZQuestFileReader(uint8_t* data, uint64_t length);

  /*!
   * \brief ZQuestFileReader
   * \param filename
   */
  ZQuestFileReader(const std::string& filename);

  /*!
   * \brief read
   * \return
   */
  ZQuestFile* read();
};

} // namespace io
} // namespace athena

#endif // __ZQUESTFILEREADER_HPP__

#endif // ATHENA_NO_ZQUEST
