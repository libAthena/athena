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

#include "Athena/WiiSave.hpp"
#include "Athena/WiiFile.hpp"
#include "Athena/WiiBanner.hpp"
#include "Athena/MemoryReader.hpp"
#include "Athena/MemoryWriter.hpp"
#include "Athena/Utility.hpp"
#include "aes.hpp"
#include "ec.h"
#include "md5.h"
#include "sha1.h"

#include <stdio.h>
#include <vector>
#include <string.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>


namespace Athena
{

WiiSave::WiiSave()
    : m_root(NULL),
      m_banner(NULL)

{
}

WiiSave::~WiiSave()
{
    delete m_root;
    delete m_banner;
    m_banner = NULL;

}

void WiiSave::addFile(WiiFile* file)
{
    m_root->addChild(file);
}

void WiiSave::setRoot(WiiFile* root)
{
    if (root != m_root)
        delete m_root;

    m_root = root;
}

WiiFile* WiiSave::file(const std::string& filepath)
{
    if (filepath.empty())
        return nullptr;

    std::string cleanPath(filepath);

    while (cleanPath.at(0) == '/')
        cleanPath.erase(cleanPath.begin());

    return m_root->child(cleanPath);
}

atUint32 WiiSave::fileCount() const
{
    return m_root->fileCount();
}

WiiFile* WiiSave::root()
{
    return m_root;
}

void WiiSave::setBanner(WiiBanner* banner)
{
    m_banner = banner;
}

WiiBanner* WiiSave::banner() const
{
    return m_banner;
}

std::vector<WiiFile*> WiiSave::allFiles() const
{
    return m_root->allChildren();
}

} // zelda

#endif // ATHENA_NO_SAVES
