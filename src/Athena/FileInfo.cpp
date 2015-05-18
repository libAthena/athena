#include "Athena/FileInfo.hpp"
#include "Athena/Utility.hpp"
#include "Athena/FileWriter.hpp"
#include "Athena/FileReader.hpp"
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>
#include <wchar.h>
#endif

#ifdef _MSC_VER
#include <functional>
#include <locale>
#define stat64 __stat64
#define realpath(__name, __resolved) _fullpath((__name), (__resolved), 4096)
#endif

namespace Athena
{

FileInfo::FileInfo(const std::string& path)
    : m_path(path)
{
}

std::string FileInfo::absolutePath() const
{
    std::string path = absoluteFilePath();
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos)
        pos = path.find_last_of('\\');
    if (pos == std::string::npos)
        return path;

    return path.substr(0, pos+1);
}

std::string FileInfo::absoluteFilePath() const
{
    char ret[4096];
    realpath(m_path.c_str(), ret);
    return ret;
}

std::string FileInfo::filename() const
{
    size_t pos = m_path.find_last_of('/');
    if (pos == std::string::npos)
        pos = m_path.find_last_of('\\');
    if (pos == std::string::npos)
        return m_path;
    return m_path.substr(pos + 1);
}

std::string FileInfo::extension() const
{
    size_t pos = m_path.find_last_of('.');
    if (pos == std::string::npos)
        return std::string();

    return m_path.substr(pos + 1);
}

atUint64 FileInfo::size() const
{
    return utility::fileSize(m_path);
}

bool FileInfo::exists() const
{
    struct stat64 st;
    int e = stat64(m_path.c_str(), &st);
    if (e < 0)
        return false;

    return (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode));
}

bool FileInfo::isLink() const
{
    struct stat64 st;
    int e = stat64(m_path.c_str(), &st);
    if (e < 0)
        return false;

    return (S_ISLNK(st.st_mode));
}

bool FileInfo::isFile() const
{
    struct stat64 st;
    int e = stat64(m_path.c_str(), &st);
    if (e < 0)
        return false;

    return (S_ISREG(st.st_mode));
}

bool FileInfo::touch() const
{
#ifndef _WIN32
    struct stat st;
    struct timespec newTimes[2];

    if (stat(m_path.c_str(), &st) < 0) {
        (void)Athena::io::FileWriter(m_path);
        return true;
    }

    /* keep atime unchanged */
    newTimes[0] = st.st_atim;

    /* set mtime to current time */
    clock_gettime(CLOCK_REALTIME, &newTimes[1]);

    if (utimensat(AT_FDCWD, m_path.c_str(), newTimes, 0) < 0) {
        return false;
    }
#else
    FILETIME modtime;
    SYSTEMTIME st;
    HANDLE fh;
    wchar_t date[80], time[80];

    fh = CreateFileW(path, GENERIC_READ | FILE_WRITE_ATTRIBUTES, 0, NULL, CREATE_NEW, 0, NULL);
    if (fh == INVALID_HANDLE_VALUE)
        return false;

    /*
     * Use GetFileTime() to get the file modification time.
     */
    if (GetFileTime(fh, NULL, NULL, &modtime) == 0)
    {
        CloseHandle(fh);
        return false;
    }

    FileTimeToSystemTime(&modtime, &st);
    if (GetDateFormatW(LOCALE_USER_DEFAULT, DATE_LONGDATE, &st, NULL, date, sizeof date / sizeof date[0]) == 0 ||
            GetTimeFormatW(LOCALE_USER_DEFAULT, 0, &st, NULL, time, sizeof time / sizeof time[0]) == 0)
    {
        CloseHandle(fh);
        return false;
    }

    /*
     * Use SetFileTime() to change the file modification time
     * to the current time.
     */
    GetSystemTime(&st);
    if (GetDateFormatW(LOCALE_USER_DEFAULT, DATE_LONGDATE, &st, NULL, date, sizeof date / sizeof date[0]) == 0 ||
            GetTimeFormatW(LOCALE_USER_DEFAULT, 0, &st, NULL, time, sizeof time / sizeof time[0]) == 0)
    {
        CloseHandle(fh);
        return false;
    }
    SystemTimeToFileTime(&st, &modtime);
    if (SetFileTime(fh, NULL, NULL, &modtime) == 0)
    {
        CloseHandle(fh);
        return false;
    }

    CloseHandle(fh);
#endif

    return true;
}

}
