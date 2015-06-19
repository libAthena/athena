#ifndef INVALIDDATAEXCEPTION_HPP
#define INVALIDDATAEXCEPTION_HPP

#include "Athena/Exception.hpp"
#include <sstream>

namespace Athena
{
namespace error
{
/*! \class InvalidDataException
 *  \brief An exception thrown on Invalid Data calls.
 *
 *  This should only be thrown when the library tries to
 *  e.g pass a NULL pointer to a function which requires a valid pointer.
 *  <br />
 *  It is <b>NOT</b> appropriate to use <b>throw new</b> so avoid doing so,
 *  keeping things on the stack as much as possible is very important for speed.
 */
class InvalidDataException : public Exception
{
public:
    inline InvalidDataException(const std::string& error, const std::string& file, const std::string& function, const int line)
        : Exception(("InvalidDataException") + error, file, function, line)
    {
        m_exceptionName = "InvalidDataException";
    }
};
} // error
} // Athena

#ifdef _MSC_VER
#define THROW_INVALID_DATA_EXCEPTION(args, ...) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, __VA_ARGS__);  return; } \
    else { std::string msg = Athena::utility::sprintf(args, __VA_ARGS__); \
        throw Athena::error::InvalidDataException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
        } \
    } while(0)
#elif defined(__GNUC__)
#define THROW_INVALID_DATA_EXCEPTION(args...) \
    do  { if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, args);  return; } \
        else { std::string msg = Athena::utility::sprintf(args); \
        throw Athena::error::InvalidDataException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
        } \
    } while(0)
#endif

#ifdef _MSC_VER
#define THROW_INVALID_DATA_EXCEPTION(args, ...) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, __VA_ARGS__);  return; } \
    else { std::string msg = Athena::utility::sprintf(args, __VA_ARGS__); \
        throw Athena::error::InvalidDataException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
        } \
    } while(0)
#elif defined(__GNUC__)
#define THROW_INVALID_DATA_EXCEPTION_RETURN(ret, args...) \
    do  { if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, args);  return ret; } \
        else { std::string msg = Athena::utility::sprintf(args); \
        throw Athena::error::InvalidDataException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
        } \
    } while(0)
#endif
#endif // INVALIDDATAEXCEPTION_HPP
