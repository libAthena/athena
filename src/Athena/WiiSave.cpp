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
