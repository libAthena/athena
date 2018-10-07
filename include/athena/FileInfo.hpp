#pragma once

#include <string>

#include "athena/Global.hpp"

namespace athena
{
class FileInfo
{
public:
    explicit FileInfo(std::string_view path = {});

    std::string absolutePath() const;
    static inline std::string absolutePath(std::string_view lnk)
    { return FileInfo(lnk).absolutePath(); }

    std::string absoluteFilePath() const;
    static inline std::string absoluteFilePath(std::string_view path)
    { return FileInfo(path).absoluteFilePath(); }

    std::string filename() const;
    static inline std::string filename(std::string_view path)
    { return FileInfo(path).filename(); }

    std::string path() const { return m_path; }
    static inline std::string path(std::string_view path)
    { return FileInfo(path).path(); }

    std::string extension() const;
    static inline std::string extension(std::string_view path)
    { return FileInfo(path).extension(); }

    atUint64 size() const;
    static inline atUint64 size(std::string_view path)
    { return FileInfo(path).size(); }

    bool exists() const;
    static inline bool exists(std::string_view path)
    { return FileInfo(path).exists(); }

    bool isLink() const;
    static inline bool isLink(std::string_view lnk)
    { return FileInfo(lnk).isLink(); }
    bool isFile() const;
    static inline bool isFile(std::string_view path)
    { return FileInfo(path).isFile(); }

    bool touch() const;
    static inline bool touch(std::string_view path)
    { return FileInfo(path).touch(); }

private:
    std::string m_path;
};
}

