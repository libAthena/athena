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

#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include <string>
#include <stdarg.h>
#include "Athena/Utility.hpp"

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
        m_line(line)
    {
    }

    /*! \brief Returns the Error message of the exception
     *  \return std::string The error message
     */
    inline std::string message() const
    {
        return m_message;
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
protected:
    std::string m_message; //!< The error message string
    std::string m_file;
    std::string m_function;
    int         m_line;
};
} // error
} // Athena
#define THROW_EXCEPTION(args...) \
    do  { \
        std::string msg = Athena::utility::sprintf(args); \
        throw Athena::error::Exception(std::string("Exception: ")+msg, __FILE__, __PRETTY_FUNCTION__, __LINE__); \
    } while(0)

#endif
