#ifndef ATHENA_NO_SAVES
// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

#ifndef ALTTP_FILE_HPP
#define ALTTP_FILE_HPP

#include "Athena/Types.hpp"
#include <vector>

namespace Athena
{

class ALTTPQuest;

/*! \class ALTTPFile
 *  \brief A Link to the Past data container class class
 *
 * Contains all relevant data for an A Link to the Past
 * SRM file.
 */
class ALTTPFile
{
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

} // zelda
#endif // ALTTP_FILE_HPP
#endif // ATHENA_NO_SAVES