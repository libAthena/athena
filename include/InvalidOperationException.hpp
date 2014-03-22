// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>

#ifndef __INVALID_OPERATION_EXCEPTION_HPP__
#define __INVALID_OPERATION_EXCEPTION_HPP__

#include <string>
#include "Exception.hpp"

namespace zelda
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
    inline InvalidOperationException(const std::string& error)
        : Exception(error)
    {
    }
};

} // error
} // zelda

#define THROW_INVALIDOPERATION_EXCEPTION(msg) \
    do  \
    { \
        throw zelda::error::InvalidOperationException(__LINE_STRING__ " " __FILE__ " " msg); \
    } while(0)

#endif // __INVALID_OPERATION_EXCEPTION_HPP__
