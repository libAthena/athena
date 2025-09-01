#pragma once

#include <string>

#if _WIN32 && !defined(__MINGW32__)
using mode_t = int;
#else
#include <sys/stat.h>
#endif

namespace athena {
class Dir {
public:
  explicit Dir(std::string_view path);

  std::string absolutePath() const;
  static std::string absolutePath(std::string_view path) { return Dir(path).absolutePath(); }

  bool isDir() const;
  static bool isDir(std::string_view dir) { return Dir(dir).isDir(); }

  bool cd(std::string_view path);
  bool rm(std::string_view path);
  bool touch();
  static bool mkdir(std::string_view dir, mode_t mode = 0755);
  static bool mkpath(std::string_view path, mode_t mode = 0755);

private:
  std::string m_path;
};
} // namespace athena
