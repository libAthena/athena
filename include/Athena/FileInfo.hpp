#ifndef FILEINFO_HPP
#define FILEINFO_HPP

#include <string>

#include "Athena/Global.hpp"

namespace Athena
{
class FileInfo
{
public:
    explicit FileInfo(const std::string& path = std::string());

    std::string absolutePath() const;
    static inline std::string absolutePath(const std::string& lnk)
    { return FileInfo(lnk).absolutePath(); }

    std::string absoluteFilePath() const;
    static inline std::string absoluteFilePath(const std::string& path)
    { return FileInfo(path).absoluteFilePath(); }

    std::string filename() const;
    static inline std::string filename(const std::string path)
    { return FileInfo(path).filename(); }

    std::string path() const;
    static inline std::string path(const std::string path)
    { return FileInfo(path).path(); }

    std::string extension() const;
    static inline std::string extension(const std::string path)
    { return FileInfo(path).extension(); }

    atUint64 size() const;
    static inline atUint64 size(const std::string path)
    { return FileInfo(path).size(); }

    bool exists() const;
    static inline bool exists(const std::string& path)
    { return FileInfo(path).exists(); }

    bool isLink() const;
    static inline bool isLink(const std::string& lnk)
    { return FileInfo(lnk).isLink(); }
    bool isFile() const;
    static inline bool isFile(const std::string& path)
    { return FileInfo(path).isFile(); }

    bool touch() const;
    static inline bool touch(const std::string& path)
    { return FileInfo(path).touch(); }

private:
    std::string m_path;
};
}

#endif // FILEINFO_HPP
