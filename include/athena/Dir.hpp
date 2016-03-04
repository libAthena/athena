#ifndef DIR_HPP
#define DIR_HPP

#include "athena/FileInfo.hpp"
#include <stdio.h>

#if _WIN32
typedef int mode_t;
#endif

namespace athena
{
class Dir
{
public:
    explicit Dir(const std::string& path);

    std::string absolutePath() const;
    static inline std::string absolutePath(const std::string& path)
    { return Dir(path).absolutePath(); }

    bool isDir() const;
    static bool isDir(const std::string dir)
    { return Dir(dir).isDir(); }

    std::vector<FileInfo> files() const;

    bool cd(const std::string& path);
    bool rm(const std::string& path);
    bool touch();
    static bool mkdir(const std::string& dir, mode_t mode = 0755);
    static bool mkpath(const std::string& path, mode_t mode = 0755);
private:
    std::string m_path;
};
}

#endif // DIR_HPP
