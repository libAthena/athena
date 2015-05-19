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
        enum AmmoType
        {
            Arrows,
            Bombs,
            Seeds
        };

        SkywardSwordQuest(atUint8* data, atUint32 len);

        void setPlayerName(const std::string& name);
        std::string playerName() const;

        void setRupeeCount(atUint16 value);
        atUint16 rupeeCount();
        void setAmmoCount(AmmoType type, atUint32 count);
        atUint32 ammoCount(AmmoType type);
        void setMaxHP(atUint16 val);
        atUint16 maxHP();
        float maxHearts();
        void setSpawnHP(atUint16 val);
        atUint16 spawnHP();
        float spawnHearts();
        void setCurrentHP(atUint16 val);
        atUint16 currentHP();
        float currentHearts();
        std::string currentLocation();
        std::string currentArea();
        std::string currentLocationCopy();

        void setSkipData(const atUint8* data);
        atUint8* skipData() const;


        atUint32 slotChecksum();
        atUint32 skipChecksum();
        void fixChecksums();

        void setNew(bool isNew);
        bool isNew() const;
    private:
        atUint8* m_skipData;
    };


    } // zelda
#endif // SSQUEST_HPP
#endif // ATHENA_NO_SAVES
