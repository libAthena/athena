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

#ifndef SSFILE_HPP
#define SSFILE_HPP

#include "Athena/Global.hpp"

// standard lib
#include <vector>

namespace Athena
{
class SkywardSwordQuest;

enum class Region
{
    NTSC,
    NTSCJ,
    PAL
};

class SkywardSwordFile
{
public:
    enum MagicNumbers
    {
        USMagic = 0x534F5545,
        JAMagic = 0x534F554A,
        EUMagic = 0x534F5550
    };

    SkywardSwordFile();
    SkywardSwordFile(std::vector<SkywardSwordQuest*> quests);
    ~SkywardSwordFile();

    void addQuest(SkywardSwordQuest* q);
    SkywardSwordQuest* quest(atUint32 id);
    std::vector<SkywardSwordQuest*> questList() const;

    void setRegion(Region region);
    Region region() const;

private:
    Region m_region;
    // A vector is a bit overkill
    std::vector<SkywardSwordQuest*> m_quests;
    atUint32 m_numQuests;
};

}
#endif // SSFILE_HPP
#endif // ATHENA_NO_SAVES
