#include "Athena/Dir.hpp"
#include <sys/stat.h>
#include <stdlib.h>
#include <limits.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#ifndef _WIN32
#include <dirent.h>
#include <unistd.h>
#else
#include <time.h>
#include <direct.h>
#endif

#ifdef _MSC_VER
#define stat64 __stat64
#define realpath(__name, __resolved) _fullpath((__name), (__resolved), 4096)
#endif

namespace Athena
{
Dir::Dir(const std::string &path)
    : m_path(path)
{
}

std::string Dir::absolutePath() const
{
    return FileInfo(m_path).absoluteFilePath();
}

bool Dir::isDir() const
{
    stat64_t st;
    int e = stat64(m_path.c_str(), &st);
    if (e < 0)
        return false;

    return (S_ISDIR(st.st_mode));
}

bool Dir::cd(const std::string& path)
{
    Dir tmp(path);
    if (tmp.isDir())
    {
        m_path = path;
        return true;
    }

    return false;
}

bool Dir::rm(const std::string& path)
{
    return !(remove((m_path + "/" + path).c_str()) < 0);
}

bool Dir::touch()
{
    srand(time(NULL));
    atUint64 tmp = utility::rand64();
    std::string tmpFile = utility::sprintf("%" PRIX64 ".tmp", tmp);
    bool ret = FileInfo(m_path + "/" + tmpFile).touch();
    if (ret)
        return rm(tmpFile);
    return false;
}

bool Dir::mkdir(const std::string& dir, mode_t mode)
{
#if _WIN32
    return !(::_mkdir(dir.c_str()) < 0);
#else    
    return !(::mkdir(dir.c_str(), mode) < 0);
#endif
}

bool Dir::mkpath(const std::string& path, mode_t mode)
{
    std::vector<std::string> dirs = utility::split(path, '/');
    if (dirs.empty())
        dirs = utility::split(path, '\\');
    if (dirs.empty())
        return false;

    bool ret = false;
    std::string newPath;
    for (const std::string& dir : dirs)
    {
        if (dir.size() == 2 && dir[1] == ':')
        {
            newPath += dir + "//";
            continue;
        }
        newPath += "/" + dir;
        ret = mkdir(newPath, mode);
    }

    // we only care if the last directory was created
    return ret;
}

}

