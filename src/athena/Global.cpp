#include "athena/Global.hpp"
#include "athena/Utility.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const athena::SeekOrigin& origin) {
  switch (origin) {
  case athena::SeekOrigin::Begin:
    os << "Begin";
    break;

  case athena::SeekOrigin::Current:
    os << "Current";
    break;

  case athena::SeekOrigin::End:
    os << "End";
    break;
  }

  return os;
}

std::ostream& operator<<(std::ostream& os, const athena::Endian& endian) {
  switch (endian) {
  case athena::Endian::Little:
    os << "LittleEndian";
    break;

  case athena::Endian::Big:
    os << "BigEndian";
    break;
  }

  return os;
}

static void __defaultExceptionHandler(athena::error::Level level, const char* file, const char* function, int line,
                                      fmt::string_view fmt, fmt::format_args args) {
  std::string levelStr;
  switch (level) {
  case athena::error::Level::Warning:
    levelStr = "[WARNING] ";
    break;
  case athena::error::Level::Error:
    levelStr = "[ERROR  ] ";
    break;
  case athena::error::Level::Fatal:
    levelStr = "[FATAL  ] ";
    break;
  default:
    break;
  }

  std::string msg = fmt::vformat(fmt, args);
  std::cerr << levelStr << " " << file << " " << function << "(" << line << "): " << msg << std::endl;
}

static atEXCEPTION_HANDLER g_atExceptionHandler = __defaultExceptionHandler;

atEXCEPTION_HANDLER atGetExceptionHandler() { return g_atExceptionHandler; }

void atSetExceptionHandler(atEXCEPTION_HANDLER func) {
  if (func)
    g_atExceptionHandler = func;
  else
    g_atExceptionHandler = __defaultExceptionHandler;
}
