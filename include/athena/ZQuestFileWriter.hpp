#ifndef ATHENA_NO_ZQUEST
#ifndef __ZQUESTFILEWRITER_HPP__
#define __ZQUESTFILEWRITER_HPP__

#include "athena/MemoryWriter.hpp"

namespace athena {
class ZQuestFile;

namespace io {

/*!
 * \brief The ZQuestFileWriter class
 */
class ZQuestFileWriter : protected MemoryCopyWriter {
public:
  /*!
   * \brief ZQuestFileWriter
   * \param data
   * \param length
   */
  ZQuestFileWriter(uint8_t* data, uint64_t length);

  /*!
   * \brief ZQuestFileWriter
   * \param filename
   */
  ZQuestFileWriter(const std::string& filename);

  /*!
   * \brief write
   * \param quest
   * \param compress
   */
  void write(ZQuestFile* quest, bool compress = true);
};

} // namespace io
} // namespace athena
#endif // __ZQUESTFILEWRITER_HPP__

#endif // ATHENA_NO_ZQUEST
