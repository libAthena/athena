#ifndef IOEXCEPTION_HPP
#define IOEXCEPTION_HPP

#include "Athena/Exception.hpp"


namespace Athena
{
namespace error
{
/*! \class IOException
 *  \brief An excpeption thrown on inappropriate IO calls.
 *
 *  This should only be thrown when the library tries to write to a buffer
 *  e.g when the position is greater than the position and the stream
 *  is not set to autoresize.<br />
 *  <br />
 *  It is <b>NOT</b> appropriate to use <b>throw new</b> so avoid doing so,
 *  keeping things on the stack as much as possible is very important for speed.
 */
class IOException : public Exception
{
public:
    /*! \brief The constructor for an IOException
     *  \param message The error message to throw
     */
    inline IOException(const std::string& message, const std::string& file, const std::string& function, const int line) :
        Exception(message, file, function, line)
    {
        m_exceptionName = "IOException";
    }
};

} // error
} // Athena

#ifdef _MSC_VER
#define THROW_IO_EXCEPTION(args, ...) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, __VA_ARGS__);  return; \
    } else {
std::string msg = Athena::utility::sprintf(args, __VA_ARGS__);
\
throw Athena::error::IOException(std::string("IOException: ") + msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__);
\
} \

} while (0)
#elif defined(__GNUC__)
#define THROW_IO_EXCEPTION(args...) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, args);  return; \
    } else { std::string msg = Athena::utility::sprintf(args); \
        throw Athena::error::IOException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
        } \
    } while(0)
#endif

#ifdef _MSC_VER
#define THROW_IO_EXCEPTION_RETURN(ret, args, ...) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, __VA_ARGS__);  return ret; \
    } else {
    std::string msg = Athena::utility::sprintf(args, __VA_ARGS__);

\
throw Athena::error::IOException(std::string("IOException: ") + msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__);
\
} \

} while (0)
#elif defined(__GNUC__)
#define THROW_IO_EXCEPTION_RETURN(ret, args...) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, args);  return ret; \
    } else { std::string msg = Athena::utility::sprintf(args); \
        throw Athena::error::IOException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
    } \
    } while(0)
#endif

#endif // IOEXCEPTION_HPP
