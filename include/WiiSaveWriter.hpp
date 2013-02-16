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
#ifndef __WII_SAVE_WRITER_HPP__
#define __WII_SAVE_WRITER_HPP__

#include <Types.hpp>
#include <utility.hpp>
#include <BinaryWriter.hpp>

class WiiSave;
class WiiBanner;
class WiiFile;
class WiiImage;

class WiiSaveWriter : public BinaryWriter
{
public:
    WiiSaveWriter(const std::string&);

    bool writeSave(WiiSave* save, Uint8* macAddress, Uint32 ngId, Uint8* ngPriv, Uint8* ngSig, Uint32 ngKeyId, const std::string& filepath = "");

private:
    void writeBanner(WiiBanner* banner);
    Uint32 writeFile(WiiFile* file);
    void writeImage(WiiImage* image);
    void writeCerts(Uint32 filesSize, Uint32 ngId, Uint8* ngPriv, Uint8* ngSig, Uint32 ngKeyId);
};

#endif // __WII_SAVE_WRITER_HPP__
