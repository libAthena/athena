#include "athena/FileInfo.hpp"

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include "athena/FileReader.hpp"
#include "athena/FileWriter.hpp"
#include "athena/Utility.hpp"

#ifndef _WIN32
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#endif

#if !(defined(HW_DOL) || defined(HW_RVL) || defined(_WIN32))
#include <fcntl.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <cwchar>
#endif

#ifdef _MSC_VER
#include <functional>
#include <locale>
#define realpath(__name, __resolved) _fullpath((__resolved), (__name), 4096)
#endif

namespace athena {

FileInfo::FileInfo(std::string_view path) : m_path(path) {}

std::string FileInfo::absolutePath() const {
  std::string path = absoluteFilePath();
  size_t pos = path.find_last_of('/');
  if (pos == std::string::npos)
    pos = path.find_last_of('\\');
  if (pos == std::string::npos)
    return path;

  return path.substr(0, pos + 1);
}

std::string FileInfo::absoluteFilePath() const {
  char ret[4096];
  realpath(m_path.c_str(), ret);
  return ret;
}

std::string FileInfo::filename() const {
  size_t pos = m_path.find_last_of('/');
  if (pos == std::string::npos)
    pos = m_path.find_last_of('\\');
  if (pos == std::string::npos)
    return m_path;
  return m_path.substr(pos + 1);
}

std::string FileInfo::extension() const {
  size_t pos = m_path.find_last_of('.');
  if (pos == std::string::npos)
    return std::string();

  return m_path.substr(pos + 1);
}

atUint64 FileInfo::size() const { return utility::fileSize(m_path); }

bool FileInfo::exists() const {
  atStat64_t st;
  int e = atStat64(m_path.c_str(), &st);

  if (e < 0)
    return false;

  return (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode));
}

bool FileInfo::isLink() const {
  atStat64_t st;
  int e = atStat64(m_path.c_str(), &st);
  if (e < 0)
    return false;

  return (S_ISLNK(st.st_mode));
}

bool FileInfo::isFile() const {
  atStat64_t st;
  int e = atStat64(m_path.c_str(), &st);
  if (e < 0)
    return false;

  return (S_ISREG(st.st_mode));
}

bool FileInfo::touch() const {
#if defined(__GNUC__) && !(defined(HW_DOL) || defined(HW_RVL) || defined(GEKKO))
  atStat64_t st;
  if (atStat64(m_path.c_str(), &st) < 0) {
    (void)athena::io::FileWriter(m_path);
    return true;
  }
  if (utimes(m_path.c_str(), nullptr) < 0) {
    return false;
  }
#elif defined(_WIN32)
  FILETIME modtime;
  SYSTEMTIME st;
  HANDLE fh;
  wchar_t date[80], time[80];

#if !WINDOWS_STORE
  fh = CreateFileA(m_path.c_str(), GENERIC_READ | FILE_WRITE_ATTRIBUTES, 0, nullptr, CREATE_NEW, 0, nullptr);

  if (fh == INVALID_HANDLE_VALUE)
    return false;

  /*
   * Use GetFileTime() to get the file modification time.
   */
  if (GetFileTime(fh, nullptr, nullptr, &modtime) == 0) {
    CloseHandle(fh);
    return false;
  }

  FileTimeToSystemTime(&modtime, &st);
  if (GetDateFormatW(LOCALE_USER_DEFAULT, DATE_LONGDATE, &st, nullptr, date, sizeof date / sizeof date[0]) == 0 ||
      GetTimeFormatW(LOCALE_USER_DEFAULT, 0, &st, nullptr, time, sizeof time / sizeof time[0]) == 0) {
    CloseHandle(fh);
    return false;
  }

  /*
   * Use SetFileTime() to change the file modification time
   * to the current time.
   */
  GetSystemTime(&st);
  if (GetDateFormatW(LOCALE_USER_DEFAULT, DATE_LONGDATE, &st, nullptr, date, sizeof date / sizeof date[0]) == 0 ||
      GetTimeFormatW(LOCALE_USER_DEFAULT, 0, &st, nullptr, time, sizeof time / sizeof time[0]) == 0) {
    CloseHandle(fh);
    return false;
  }
  SystemTimeToFileTime(&st, &modtime);
  if (SetFileTime(fh, nullptr, nullptr, &modtime) == 0) {
    CloseHandle(fh);
    return false;
  }

  CloseHandle(fh);
#endif
#elif (defined(HW_RVL) || defined(HW_DOL)) && defined(GEKKO)
  // Generic portable version, not extremely reliable but does work
  atUint64 val = 0xCDCDCDCDCD;
  {
    athena::io::FileReader reader(m_path.c_str());
    if (reader.isOpen())
      val = reader.readUint64();
  }

  {
    athena::io::FileWriter writer(m_path, false);
    if (val != 0xCDCDCDCDCD && writer.isOpen())
      writer.writeUint64(val);
    else if (!writer.isOpen())
      return false;
  }

#endif

  return true;
}

} // namespace athena
