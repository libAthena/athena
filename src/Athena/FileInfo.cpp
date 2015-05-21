#include "Athena/FileInfo.hpp"
#include "Athena/Utility.hpp"
#include "Athena/FileWriter.hpp"
#include "Athena/FileReader.hpp"
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

#if !(defined(HW_DOL) || defined(HW_RVL) || defined(_WIN32))
#include <fcntl.h>
#endif

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
    struct stat64_t st;
    int e = stat64(m_path.c_str(), &st);

    if (e < 0)
        return false;

    return (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode));
}

bool FileInfo::isLink() const
{
    struct stat64_t st;
    int e = stat64(m_path.c_str(), &st);
    if (e < 0)
        return false;

    return (S_ISLNK(st.st_mode));
}

bool FileInfo::isFile() const
{
    stat64_t st;
    int e = stat64(m_path.c_str(), &st);
    if (e < 0)
        return false;

    return (S_ISREG(st.st_mode));
}

bool FileInfo::touch() const
{
#if defined(__GNUC__) && !(defined(HW_DOL) || defined(HW_RVL) || defined(GEKKO))
    stat64_t st;
    if (stat64(m_path.c_str(), &st) < 0) {
        (void)Athena::io::FileWriter(m_path);
        return true;
    }
    if (utimensat(AT_FDCWD, m_path.c_str(), NULL, 0) < 0) {
        return false;
    }
#elif defined(_WIN32)
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
#elif (defined(HW_RVL) || defined(HW_DOL)) && defined(GEKKO)
    // Generic portable version, not extremely reliable but does work
    atUint64 val = 0xCDCDCDCDCD;
    try
    {
        Athena::io::FileReader reader(m_path.c_str());
        val = reader.readUint64();
    }
    catch(...)
    {
    }

    try
    {
        Athena::io::FileWriter writer(m_path, false);
        if (val != 0xCDCDCDCDCD)
            writer.writeUint64(val);
    }
    catch(...)
    {
        return false;
    }

#endif

    return true;
}

}
