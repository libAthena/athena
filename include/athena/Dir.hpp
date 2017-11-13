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
    explicit Dir(std::string_view path);

    std::string absolutePath() const;
    static inline std::string absolutePath(std::string_view path)
    { return Dir(path).absolutePath(); }

    bool isDir() const;
    static bool isDir(std::string_view dir)
    { return Dir(dir).isDir(); }

    std::vector<FileInfo> files() const;

    bool cd(std::string_view path);
    bool rm(std::string_view path);
    bool touch();
    static bool mkdir(std::string_view dir, mode_t mode = 0755);
    static bool mkpath(std::string_view path, mode_t mode = 0755);
private:
    std::string m_path;
};
}

#endif // DIR_HPP
