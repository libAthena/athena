#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string>
#include <stdarg.h>
#include "Athena/Utility.hpp"
#include "Athena/Global.hpp"

#define __STRX(x) #x
#define __STR(x) __STRX(x)
#define __LINE_STRING__ __STR(__LINE__)

namespace Athena
{
namespace error
{
/*! \class Exception
 *  \brief The baseclass for all Exceptions.
 *
 *  <b>Do Not</b> use Exception directly, instead create an appropriate
 * Exception class and inherit from this baseclass.
 */
class Exception
{
public:
    /*! \brief The constructor for an Exception
     *  \param message The error message to throw
     */
    inline Exception(const std::string& message, const std::string& file, const std::string& function, const int line) :
        m_message(message),
        m_file(file),
        m_function(function),
        m_line(line),
        m_exceptionName("Exception")
    {
    }

    /*! \brief Returns the Error message of the exception
     *  \return std::string The error message
     */
    inline std::string message() const
    {
        return m_exceptionName + (m_message.empty() ? "" : ": " + m_message);
    }

    inline std::string file() const
    {
        return m_file;
    }

    inline std::string function() const
    {
        return m_function;
    }

    inline int line() const
    {
        return m_line;
    }

    inline std::string formattedMessage() const
    {
        return Athena::utility::sprintf("%s : %s (%i) %s", m_file.c_str(), m_function.c_str(), m_line, message().c_str());
    }
protected:
    std::string m_message; //!< The error message string
    std::string m_file;
    std::string m_function;
    int         m_line;
    std::string m_exceptionName;
};
} // error
} // Athena
#ifdef _MSC_VER
#define THROW_EXCEPTION(args,...) \
do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, __VA_ARGS__);  return; \
    } else { std::string msg = Athena::utility::sprintf(__VA_ARGS__); \
        throw Athena::error::Exception(std::string("Exception: ")+msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
    } \
} while(0)
#elif defined(__GNUC__)
#define THROW_EXCEPTION(args...) \
do  { \
    if (atGetExceptionHandler()) { atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, args); return; \
    } else { \
        std::string msg = Athena::utility::sprintf(args); \
        throw Athena::error::Exception(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
    } \
} while(0)
#endif

#ifdef _MSC_VER
#define THROW_EXCEPTION_RETURN(ret, args,...) \
do  { \
    if (atGetExceptionHandler()) \
    { \
        atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, __VA_ARGS__); \
        return ret; \
    } else { \
        std::string msg = Athena::utility::sprintf(__VA_ARGS__); \
        throw Athena::error::Exception(std::string("Exception: ")+msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
    } \
} while(0)
#elif defined(__GNUC__)
#define THROW_EXCEPTION_RETURN(ret, args...) \
do  { \
    if (atGetExceptionHandler()) { atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, args); return ret; \
    } else { \
        std::string msg = Athena::utility::sprintf(args); \
        throw Athena::error::Exception(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
    } \
} while(0)
#endif

#endif // EXCEPTION_HPP
