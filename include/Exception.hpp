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

#ifndef __EXCEPTION_HPP__
#define __EXCEPTION_HPP__

#include <string>

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
    Exception(const std::string& message) :
        m_message(message)
    {};

    /*! \brief Returns the Error message of the exception
     *  \return std::string The error message
     */
    std::string message() const
    {
        return m_message;
    };
protected:
    std::string m_message;
};

#endif
