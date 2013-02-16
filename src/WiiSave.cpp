#include "WiiSave.h"
#include "WiiFile.h"
#include "WiiBanner.h"
#include "BinaryReader.hpp"
#include "BinaryWriter.hpp"
#include "IOException.hpp"
#include "aes.h"
#include "ec.h"
#include <utility.hpp>
#include "md5.h"
#include "sha1.h"

#include <stdio.h>
#include <vector>
#include <string.h>
#include <sys/stat.h>
#include <iostream>
#include <iomanip>


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
