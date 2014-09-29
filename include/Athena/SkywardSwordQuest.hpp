#if !defined(ATHENA_NO_SAVES) && !defined(ATHENA_NO_ZQUEST)
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

#ifndef SKYWARDSWORDQUEST_HPP
#define SKYWARDSWORDQUEST_HPP

#include "Athena/Global.hpp"
#include "Athena/ZQuestFile.hpp"

namespace Athena
{

// TODO: Handle game specific data
class SkywardSwordQuest : public ZQuestFile
{
public:
    SkywardSwordQuest(atUint8* data, atUint32 len);

    // TODO: Is len really needed?
    void setSkipData(const atUint8* data, atUint32 len = 0x24);
    atUint8* skipData() const;

    atUint32 skipLength() const;

private:
    atUint8* m_skipData;
    atUint32 m_skipLength;
};


} // zelda
#endif // SSQUEST_HPP
#endif // ATHENA_NO_SAVES
