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

#ifndef __IOEXCEPTION_HPP__
#define __IOEXCEPTION_HPP__

#include "Exception.hpp"


namespace zelda
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
    IOException(const std::string& message) :
        Exception("IOException: " + message)
    {};
};

} // zelda
#endif
