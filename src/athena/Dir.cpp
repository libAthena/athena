#include "athena/Dir.hpp"

#include <climits>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>

#define __STDC_FORMAT_MACROS
#include <cinttypes>

#include "athena/FileInfo.hpp"
#include "athena/Utility.hpp"

#ifndef _WIN32
#include <dirent.h>
#include <unistd.h>
#else
#include <direct.h>
#endif

#ifdef _MSC_VER
#define realpath(__name, __resolved) _fullpath((__name), (__resolved), 4096)
#endif

namespace athena {
Dir::Dir(std::string_view path) : m_path(path) {}

std::string Dir::absolutePath() const { return FileInfo(m_path).absoluteFilePath(); }

bool Dir::isDir() const {
  atStat64_t st;
  int e = atStat64(m_path.c_str(), &st);
  if (e < 0)
    return false;

  return (S_ISDIR(st.st_mode));
}

bool Dir::cd(std::string_view path) {
  Dir tmp(path);
  if (tmp.isDir()) {
    m_path = path;
    return true;
  }

  return false;
}

bool Dir::rm(std::string_view path) { return !(remove((m_path + "/" + path.data()).c_str()) < 0); }

bool Dir::touch() {
  std::srand(std::time(nullptr));
  atUint64 tmp = utility::rand64();
  std::string tmpFile = std::format("{:016X}.tmp", tmp);
  bool ret = FileInfo(m_path + "/" + tmpFile).touch();
  if (ret)
    return rm(tmpFile);
  return false;
}

bool Dir::mkdir(std::string_view dir, mode_t mode) {
#if _WIN32
  return !(::_mkdir(dir.data()) < 0);
#else
  return !(::mkdir(dir.data(), mode) < 0);
#endif
}

bool Dir::mkpath(std::string_view path, mode_t mode) {
  std::vector<std::string> dirs = utility::split(path, '/');
  if (dirs.empty())
    dirs = utility::split(path, '\\');
  if (dirs.empty())
    return false;

  bool ret = false;
  std::string newPath;
  for (const std::string& dir : dirs) {
    if (dir.size() == 2 && dir[1] == ':') {
      newPath += dir + "//";
      continue;
    }
    newPath += "/" + dir;
    ret = mkdir(newPath, mode);
  }

  // we only care if the last directory was created
  return ret;
}

} // namespace athena
