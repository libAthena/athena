#pragma once

#include <string>

#include "athena/Types.hpp"

namespace athena {
class FileInfo {
public:
  explicit FileInfo(std::string_view path = {});

  std::string absolutePath() const;
  static std::string absolutePath(std::string_view lnk) { return FileInfo(lnk).absolutePath(); }

  std::string absoluteFilePath() const;
  static std::string absoluteFilePath(std::string_view path) { return FileInfo(path).absoluteFilePath(); }

  std::string filename() const;
  static std::string filename(std::string_view path) { return FileInfo(path).filename(); }

  std::string path() const { return m_path; }
  static std::string path(std::string_view path) { return FileInfo(path).path(); }

  std::string extension() const;
  static std::string extension(std::string_view path) { return FileInfo(path).extension(); }

  atUint64 size() const;
  static atUint64 size(std::string_view path) { return FileInfo(path).size(); }

  bool exists() const;
  static bool exists(std::string_view path) { return FileInfo(path).exists(); }

  bool isLink() const;
  static bool isLink(std::string_view lnk) { return FileInfo(lnk).isLink(); }
  bool isFile() const;
  static bool isFile(std::string_view path) { return FileInfo(path).isFile(); }

  bool touch() const;
  static bool touch(std::string_view path) { return FileInfo(path).touch(); }

private:
  std::string m_path;
};
} // namespace athena
