#include "Athena/Global.hpp"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

std::ostream& operator<<(std::ostream& os, const Athena::SeekOrigin& origin)
{
    switch (origin)
    {
        case Athena::SeekOrigin::Begin:
            os << "Begin";
            break;

        case Athena::SeekOrigin::Current:
            os << "Current";
            break;

        case Athena::SeekOrigin::End:
            os << "End";
            break;
    }

    return os;
}


std::ostream& operator<<(std::ostream& os, const Athena::Endian& endian)
{
    switch (endian)
    {
        case Athena::Endian::LittleEndian:
            os << "LittleEndian";
            break;

        case Athena::Endian::BigEndian:
            os << "BigEndian";
            break;
    }

    return os;
}


static void __defaultExceptionHandler(const Athena::error::Level& level, const char* file, const char* function, int line, const char* fmt, ...)
{
    std::string levelStr;
    switch(level)
    {
        case Athena::error::LevelWarning:
            levelStr = "[WARNING] ";
            break;
        case Athena::error::LevelError:
            levelStr = "[ERROR  ] ";
            break;
        case Athena::error::LevelFatal:
            levelStr = "[FATAL  ] ";
            break;
        default: break;
    }

    va_list vl;
    va_start(vl, fmt);
    std::string msg = Athena::utility::vsprintf(fmt, vl);
    va_end(vl);
    std::cerr << levelStr << " " << file << " " << function << "(" << line << "): " << msg << std::endl;
}

static atEXCEPTION_HANDLER g_atExceptionHandler = __defaultExceptionHandler;

atEXCEPTION_HANDLER atGetExceptionHandler()
{
    return g_atExceptionHandler;
}


void atSetExceptionHandler(atEXCEPTION_HANDLER func)
{
    if (func)
        g_atExceptionHandler = func;
    else
        g_atExceptionHandler = __defaultExceptionHandler;
}
