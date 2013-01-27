#ifndef __FILESTREAM_HPP__
#define __FILESTREAM_HPP__

#include "Stream.hpp"
#include <string>
#include <vector>

class TextStream : public Stream
{
public:
    enum FileMode { Open, Create, OpenOrCreate = Open|Create, Truncate, Append };
    enum AccessMode { ReadOnly, WriteOnly, ReadWrite };
    TextStream(const std::string& filename, FileMode fileMode = Open, AccessMode accessMode = ReadWrite);

    void                     save(const std::string& filename = "");
    std::string              readLine();
    void                     writeLine(const std::string& str);

    std::vector<std::string> readLines(Uint32 numLines);
    void                     writeLines(std::vector<std::string> lines);

    std::vector<std::string> readAllLines();

    std::string              readLineAt(Uint32 line);

    void                  setCurrentLine(Uint32 line);
    Uint32                   currentLine() const;
private:
    std::string m_filename;
    FileMode    m_filemode;
    AccessMode  m_accessmode;
    Uint32      m_currentLine;
    Uint32      m_startLength;
};

#endif
