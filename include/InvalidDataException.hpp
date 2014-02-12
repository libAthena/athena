#ifndef INVALIDDATAEXCEPTION_HPP
#define INVALIDDATAEXCEPTION_HPP

#include "Exception.hpp"

namespace zelda
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
    inline InvalidDataException(const std::string& error)
        : Exception("InvalidDataException: " + error)
    {
    }
};
}
}
#endif // INVALIDDATAEXCEPTION_HPP
