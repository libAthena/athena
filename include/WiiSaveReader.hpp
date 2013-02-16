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
#ifndef __WII_SAVE_READER_HPP__
#define __WII_SAVE_READER_HPP__

#include <Types.hpp>
#include <utility.hpp>
#include <BinaryReader.hpp>

class WiiSave;
class WiiBanner;
class WiiFile;
class WiiImage;

class WiiSaveReader : public BinaryReader
{
public:
    WiiSaveReader(const Uint8*, Uint64);
    WiiSaveReader(const std::string&);

    WiiSave* readSave();
private:
    WiiBanner* readBanner();
    WiiFile*   readFile();
    WiiImage*  readImage(Uint32 width, Uint32 height);
    void       readCerts(Uint32 totalSize);
};
#endif // __WII_SAVE_READER_HPP__
