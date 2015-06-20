#ifndef INVALID_OPERATION_EXCEPTION_HPP
#define INVALID_OPERATION_EXCEPTION_HPP

#include <string>
#include <stdarg.h>
#include "Athena/Exception.hpp"

namespace Athena
{
namespace error
{
/*! \class InvalidOperationException
 *  \brief An excpeption thrown on Invalid Operations calls.
 *
 *  This should only be thrown when the library tries to
 *  e.g pass a NULL pointer to a function which requires a valid pointer.
 *  <br />
 *  It is <b>NOT</b> appropriate to use <b>throw new</b> so avoid doing so,
 *  keeping things on the stack as much as possible is very important for speed.
 */
class InvalidOperationException : public Exception
{
public:
    /*! \brief The constructor for an InvalidOperationException
     *  \param error The error message to throw
     */
    inline InvalidOperationException(const std::string& message, const std::string& file, const std::string& function, const int line) :
        Exception(message, file, function, line)
    {
        m_exceptionName = "InvalidOperationException";
    }
};
} // error
} // Athena

#ifdef _MSC_VER
#define THROW_INVALID_OPERATION_EXCEPTION(args, ...) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, args, __VA_ARGS__);  return; \
    } else { \
std::string msg = Athena::utility::sprintf(args , __VA_ARGS__); \
\
throw Athena::error::InvalidOperationException(std::string("InvalidOperationException: ") + msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
\
} \
} while (0)
#elif defined (__GNUC__)
#define THROW_INVALID_OPERATION_EXCEPTION(args...) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, args);  return; \
    } else { std::string msg = Athena::utility::sprintf(args); \
        throw Athena::error::InvalidOperationException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
        } \
    } while(0)
#endif

#ifdef _MSC_VER
#define THROW_INVALID_OPERATION_EXCEPTION_RETURN(ret, args, ...) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, args, __VA_ARGS__);  return ret; \
    } else { \
    std::string msg = Athena::utility::sprintf(args, __VA_ARGS__); \
\
throw Athena::error::InvalidOperationException(std::string("InvalidOperationException: ") + msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
} \
} while (0)
#elif defined(__GNUC__)
#define THROW_INVALID_OPERATION_EXCEPTION_RETURN(ret, args...) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, args);  return ret; \
    } else { std::string msg = Athena::utility::sprintf(args); \
        throw Athena::error::InvalidOperationException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
    } \
    } while(0)
#endif
#endif // INVALID_OPERATION_EXCEPTION_HPP
