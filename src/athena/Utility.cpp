#include "athena/Utility.hpp"

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <random>
#include <sstream>

#include <sys/stat.h>
#include <sys/types.h>
#include "athena/utf8proc.h"

#ifdef _MSC_VER
#include <functional>
#include <locale>
#endif

namespace athena::utility {

void fillRandom(atUint8* rndArea, atUint64 count) {
  atUint8* buf = rndArea;
  for (atUint64 i = 0; i < count / 4; i++) {
    *(atUint32*)(buf) = rand();
    buf += 4;
  }

  atUint64 rem = count % 4;
  if (rem) {
    for (atUint64 j = 0; j < rem; j++) {
      *buf = rand();
      buf++;
    }
  }
}

static std::vector<std::string>& split(std::string_view s, char delim, std::vector<std::string>& elems) {
  std::string tmps(s);
  std::stringstream ss(tmps);
  std::string item;

  while (std::getline(ss, item, delim))
    elems.push_back(item);

  return elems;
}

std::vector<std::string> split(std::string_view s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

std::string join(const std::vector<std::string>& elems, std::string_view delims) {
  std::ostringstream ret;
  std::copy(elems.begin(), elems.end(), std::ostream_iterator<std::string>(ret, delims.data()));

  return ret.str();
}

void tolower(std::string& str) { std::transform(str.begin(), str.end(), str.begin(), ::tolower); }

void toupper(std::string& str) { std::transform(str.begin(), str.end(), str.begin(), ::toupper); }

bool parseBool(std::string_view boolean, bool* valid) {
  std::string val(boolean);
  // compare must be case insensitive
  // This is the cleanest solution since I only need to do it once
  tolower(val);

  // Check for true first
  if (val == "true" || val == "1" || val == "yes" || val == "on") {
    if (valid)
      *valid = true;

    return true;
  }

  // Now false
  if (val == "false" || val == "0" || val == "no" || val == "off") {
    if (valid)
      *valid = true;

    return false;
  }

  // Well that could've gone better

  if (valid)
    *valid = false;

  return false;
}

int countChar(std::string_view str, const char chr, int* lastOccur) {
  int ret = 0;

  int index = 0;

  for (char c : str) {
    if (c == chr) {
      if (lastOccur != nullptr)
        *lastOccur = index;

      ret++;
    }

    index++;
  }

  return ret;
}

atUint64 fileSize(std::string_view filename) {
  atStat64_t st;
  atStat64(filename.data(), &st);
  return st.st_size;
}

#ifdef _WIN32
atUint64 fileSize(std::wstring_view filename) {
  atStat64_t st;
  _wstati64(filename.data(), &st);
  return st.st_size;
}
#endif

// trim from both ends
std::string& trim(std::string& s) {
  // Find first non whitespace char in StrToTrim
  std::string::size_type first = s.find_first_not_of(' ');

  // Check whether something went wrong?
  if (first == std::string::npos) {
    first = 0;
  }

  // Find last non whitespace char from StrToTrim
  std::string::size_type last = s.find_last_not_of(' ');

  // If something didn't go wrong, Last will be recomputed to get real length of substring
  if (last != std::string::npos) {
    last = (last + 1) - first;
  }

  // Copy such a string to TrimmedString
  s = s.substr(first, last);

  return s;
}

atUint64 rand64() {
  // Combine 4 parts of low 16-bit of each rand()
  atUint64 r0 = (atUint64)rand() << 48;
  atUint64 r1 = (atUint64)rand() << 48 >> 16;
  atUint64 r2 = (atUint64)rand() << 48 >> 32;
  atUint64 r3 = (atUint64)rand() << 48 >> 48;
  return r0 | r1 | r2 | r3;
}

std::string wideToUtf8(std::wstring_view src) {
  std::string retval;
  retval.reserve(src.length());
  for (wchar_t ch : src) {
    utf8proc_uint8_t mb[4];
    utf8proc_ssize_t c = utf8proc_encode_char(utf8proc_int32_t(ch), mb);
    if (c < 0) {
      atWarning("invalid UTF-8 character while encoding");
      return retval;
    }
    retval.append(reinterpret_cast<char*>(mb), c);
  }
  return retval;
}

std::wstring utf8ToWide(std::string_view src) {
  std::wstring retval;
  retval.reserve(src.length());
  const utf8proc_uint8_t* buf = reinterpret_cast<const utf8proc_uint8_t*>(src.data());
  while (*buf) {
    utf8proc_int32_t wc;
    utf8proc_ssize_t len = utf8proc_iterate(buf, -1, &wc);
    if (len < 0) {
      atWarning("invalid UTF-8 character while decoding");
      return retval;
    }
    buf += len;
    retval += wchar_t(wc);
  }
  return retval;
}

} // namespace athena::utility
