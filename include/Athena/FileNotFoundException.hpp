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

#ifndef __FILENOTFOUNDEXCEPTION_HPP__
#define __FILENOTFOUNDEXCEPTION_HPP__

#include "Athena/Exception.hpp"

namespace Athena
{
namespace error
{
/*! \class FileNotFoundException
 *  \brief An excpeption thrown when a file could not be found at the given path.
 *
 *  This should only be thrown when the Stream is unable to open a file.<br />
 *  <br />
 *  It is <b>NOT</b> appropriate to use <b>throw new</b> so avoid doing so,
 *  keeping things on the stack as much as possible is very important for speed.
 */
class FileNotFoundException : public Exception
{
public:
    /*! \brief The constructor for an FileNotFoundException
     *  \param filename The path of the offending file.
     */
    inline FileNotFoundException(const std::string& filename, const std::string& file, const std::string& function, const int line) :
        Exception(std::string("FileNotFoundException: Could not find file \"") + filename + std::string("\", please check that it exists."), file, function, line),
        m_filename(filename)
    {}

    /*! \brief Returns the path of the offending file.
     *  \return std::string The filename of the file including the path.
     */
    inline std::string filename() const { return m_filename; }
private:
    std::string m_filename;
};
} // error
} // Athena

#define THROW_FILE_NOT_FOUND_EXCEPTION(msg) \
    do  { throw Athena::error::FileNotFoundException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); } while(0)

#endif
