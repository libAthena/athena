// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

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
        : Exception(error, file, function, line)
    {
    }
};
} // error
} // Athena

#define THROW_INVALID_DATA_EXCEPTION(args...) \
    do  { \
        std::string msg = Athena::utility::sprintf(args); \
        throw Athena::error::InvalidDataException(std::string("InvalidDataException: ")+msg, __FILE__, __PRETTY_FUNCTION__, __LINE__); \
    } while(0)

#endif // INVALIDDATAEXCEPTION_HPP
