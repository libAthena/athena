// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>

#ifndef __ALTTP_FILE_WRITER_HPP__
#define __ALTTP_FILE_WRITER_HPP__

#include <string>
#include <Types.hpp>
#include <BinaryWriter.hpp>
#include "ALTTPQuest.hpp"

class ALTTPFile;

class ALTTPFileWriter : public BinaryWriter
{
public:
    ALTTPFileWriter(Uint8*, Uint64);
    ALTTPFileWriter(const std::string&);

    void writeFile(ALTTPFile* file);
private:
    void writeRoomFlags(ALTTPRoomFlags*);
    void writeOverworldEvent(ALTTPOverworldEvent*);
    void writeDungeonItems(ALTTPDungeonItemFlags);
    Uint16 calculateChecksum(Uint32 game);
};

#endif // __ALTTP_FILE_WRITER_HPP__
