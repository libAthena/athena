#ifndef FILENOTFOUNDEXCEPTION_HPP
#define FILENOTFOUNDEXCEPTION_HPP

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
    {
        m_exceptionName = "FileNotFoundException";
    }

    /*! \brief Returns the path of the offending file.
     *  \return std::string The filename of the file including the path.
     */
    inline std::string filename() const { return m_filename; }
private:
    std::string m_filename;
};
} // error
} // Athena

#ifndef THROW_FILE_NOT_FOUND_EXCEPTION
#define THROW_FILE_NOT_FOUND_EXCEPTION(msg) \
    do  { \
            if (atGetExceptionHandler()) \
            { \
                atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, msg); \
                return; \
            } \
            else \
                throw Athena::error::FileNotFoundException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
} while(0)
#endif

#ifndef THROW_FILE_NOT_FOUND_EXCEPTION_RETURN
#define THROW_FILE_NOT_FOUND_EXCEPTION_RETURN(ret, msg) \
    do  { \
            if (atGetExceptionHandler()) \
            { \
                atGetExceptionHandler()(__FILE__, AT_PRETTY_FUNCTION, __LINE__, msg); \
                return ret; \
            } \
            else \
                throw Athena::error::FileNotFoundException(msg, __FILE__, AT_PRETTY_FUNCTION, __LINE__); \
} while(0)
#endif


#endif // FILENOTFOUNDEXCEPTION_HPP
