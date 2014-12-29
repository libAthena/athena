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
    {}
};
} // error
} // Athena

#define THROW_NOT_IMPLEMENTED_EXCEPTION() \
    do { throw Athena::error::NotImplementedException("NotImplementedException", __FILE__, AT_PRETTY_FUNCTION, __LINE__); } while(0)

#endif // NOTIMPLEMENTEDEXCEPTION_HPP
