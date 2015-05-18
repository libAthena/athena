#include "Athena/Dir.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

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
    struct stat64 st;
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

bool Dir::mkdir(const std::string& dir, mode_t mode)
{
    return !(::mkdir(dir.c_str(), mode) < 0);
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

