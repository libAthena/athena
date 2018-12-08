#pragma once

#include "athena/Global.hpp"
#include <vector>

namespace athena {

class ALTTPQuest;

/*! \class ALTTPFile
 *  \brief A Link to the Past data container class class
 *
 * Contains all relevant data for an A Link to the Past
 * SRM file.
 */
class ALTTPFile {
public:
  /*! \brief Quest Iterator
   *
   * An Iterator typedef for iterating through the Quest lists
   */
  typedef std::vector<ALTTPQuest*>::iterator QuestIter;

  /*! \brief Default constructor
   *
   *
   */
  ALTTPFile();

  /*! \brief Constructor
   *
   *  \param questList The primary quest list
   *  \param backupList The backup quest list
   */
  ALTTPFile(std::vector<ALTTPQuest*> questList, std::vector<ALTTPQuest*> backupList);

  /*! \brief Sets a quest at the given index
   *
   * \param id Index to the given quest
   * \param val The new quest to assign to the given index
   * \throw InvalidOperationException on index out of range
   */
  void setQuest(atUint32 id, ALTTPQuest* val);
  /*! \brief Returns the primary quest list
   *
   * \return The primary quest list
   */
  std::vector<ALTTPQuest*> questList() const;

  /*! \brief Returns a quest at the given index
   *
   * Returns a quest at the given index
   *
   * \return ALTTPQuest*
   * \throw InvalidOperationException on index out of range
   */
  ALTTPQuest* quest(atUint32 id) const;

  /*! \brief Returns the number of primary quests
   *
   * \return The number of quests
   */
  atUint32 questCount() const;

private:
  std::vector<ALTTPQuest*> m_quests;
  std::vector<ALTTPQuest*> m_backup;
};

} // namespace athena
