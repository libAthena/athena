#ifndef NOTIMPLEMENTEDEXCEPTION_HPP
#define NOTIMPLEMENTEDEXCEPTION_HPP

#include "Athena/Exception.hpp"

namespace Athena
{
namespace error
{
class NotImplementedException : public Exception
{
public:
    NotImplementedException(const std::string& message, const std::string& file, const std::string& function, const int line) :
        Exception(message, file, function, line)
    {
        m_exceptionName = "NotImplementedException";
    }
};
} // error
} // Athena

#define THROW_NOT_IMPLEMENTED_EXCEPTION() \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, "NotImplementedException");  return; \
    } else { \
        throw Athena::error::NotImplementedException(std::string(), __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
        } \
    } while(0)

#define THROW_NOT_IMPLEMENTED_EXCEPTION_RETURN(ret) \
    do  { \
    if (atGetExceptionHandler()) {atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, "NotImplementedException");  return ret; \
    } else { \
        throw Athena::error::NotImplementedException(std::string(), __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
        } \
    } while(0)
#endif // NOTIMPLEMENTEDEXCEPTION_HPP
