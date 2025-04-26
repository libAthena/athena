#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "athena/Global.hpp"

namespace athena {

class WiiFile;
class WiiBanner;
class WiiImage;
class BinaryReader;
class BinaryWriter;

/*! \class WiiSave
 *  \brief Wii data.bin container class
 *
 * Contains all relevant data for a Wii data.bin file.
 */
class WiiSave {
public:
  /*!
   * \brief FileIterator
   */
  typedef std::unordered_map<std::string, WiiFile*>::const_iterator FileIterator;

  /*!
   * \brief WiiSave
   */
  WiiSave();
  /*!
   * \brief ~WiiSave
   */
  virtual ~WiiSave();

  /*!
   * \brief addFile
   * \param filename
   * \param file
   */
  void addFile(WiiFile* file);
  void setRoot(WiiFile* root);
  /*!
   * \brief file
   * \param filename
   * \return
   */
  WiiFile* file(const std::string& filename);

  uint32_t fileCount() const;
  /*!
   * \brief fileList
   * \return
   */
  WiiFile* root();

  /*!
   * \brief setBanner
   * \param banner
   */
  void setBanner(WiiBanner* banner);

  /*!
   * \brief banner
   * \return
   */
  WiiBanner* banner() const;

  std::vector<WiiFile*> allFiles() const;

protected:
private:
  WiiFile* m_root;
  WiiBanner* m_banner;
};

} // namespace athena
