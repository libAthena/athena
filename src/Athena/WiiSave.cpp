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
#include "Athena/BinaryReader.hpp"
#include "Athena/BinaryWriter.hpp"
#include "Athena/Utility.hpp"
#include "aes.h"
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
    : m_banner(NULL)
{
}

WiiSave::~WiiSave()
{
    m_files.clear();

    delete m_banner;
    m_banner = NULL;

}

void WiiSave::addFile(const std::string& filepath, WiiFile* file)
{
    m_files[filepath] = file;
}

void WiiSave::setFiles(std::unordered_map<std::string, WiiFile*> files)
{
    if (files.size() <= 0)
        return;

    std::cout << "Setting file map...";
    if (m_files.size() > 0)
    {
        std::unordered_map<std::string, WiiFile*>::iterator iter = m_files.begin();

        for (;iter != m_files.end(); ++iter)
        {
            if (iter->second)
                delete iter->second;
        }

        m_files.clear();
    }

    m_files = files;
    std::cout << "done" << std::endl;
}

WiiFile* WiiSave::file(const std::string& filepath) const
{
    std::unordered_map<std::string, WiiFile*>::const_iterator iter = m_files.begin();

    for (;iter != m_files.end(); ++iter)
    {
        if (iter->first == filepath)
            return (WiiFile*)iter->second;
    }

    return NULL;
}

std::unordered_map<std::string, WiiFile*>& WiiSave::fileList()
{
    return m_files;
}

void WiiSave::setBanner(WiiBanner* banner)
{
    m_banner = banner;
}

WiiBanner* WiiSave::banner() const
{
    return m_banner;
}

} // zelda

#endif // ATHENA_NO_SAVES
