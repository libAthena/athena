#include "TextStream.hpp"
#include <stdio.h>
#include "FileNotFoundException.hpp"
#include "InvalidOperationException.hpp"
#include "IOException.hpp"

TextStream::TextStream(const std::string& filename, FileMode fileMode, AccessMode accessMode) :
    m_filename(filename),
    m_filemode(fileMode),
    m_accessmode(accessMode),
    m_currentLine(0)
{
    setAutoResizing(true);

    FILE* in;
    int length;
    in = fopen(filename.c_str(), "rb");

    if (!in)
    {
        if((fileMode & Create) != Create)
            throw FileNotFoundException(filename.c_str());

        in = fopen(filename.c_str(), "w+b");
    }

    if ((fileMode & Truncate) != Truncate)
    {
        fseek(in, 0, SEEK_END);
        length = ftell(in);
        fseek(in, 0, SEEK_SET);
        m_data = new Uint8[length];
        fread(m_data, 1, length, in);
        fclose(in);
    }
    else
    {
        length = 1;
        m_data = new Uint8[1];
    }

    if ((fileMode & Append) == Append)
        m_position = length;
    else
        m_position = 0;

    m_length = length;
    m_startLength = length;
}

void  TextStream::save(const std::string& filename)
{
    if (filename != std::string())
        m_filename = filename;

    FILE* out = fopen(m_filename.c_str(), "wb");

    if(out)
    {
        fwrite(m_data, 1, m_length, out);
        fclose(out);
    }
    else
        throw FileNotFoundException(m_filename);
}
std::string TextStream::readLine()
{
    std::string ret;
    Uint8 c;
    for (;;)
    {
        c = readByte();

        if (c == '\r' || c == '\n')
        {
            m_currentLine++;
            ret.push_back(c);
            if (*(Uint8*)(m_data + m_position + 1) == '\n')
            {
                ret.push_back('\n');
                m_currentLine++;
                m_position++; // advance position past the new line character
            }
            break;
        }

        if (c == '\0')
        {
            ret.push_back('\n');
            m_currentLine++;
            break;
        }
        ret.push_back(c);
    }
    return ret;
}


void  TextStream::writeLine(const std::string& str)
{
    this->writeBytes((Int8*)str.c_str(), str.size());
    m_currentLine++;
}

void TextStream::writeLines(std::vector<std::string> strings)
{
    for (std::string s: strings)
        writeLine(s);
}

std::vector<std::string> TextStream::readLines(Uint32 numLines)
{
    m_position = 0;
    m_currentLine = 0;
    std::vector<std::string> ret;

    while ((numLines--) > 0)
    {
        ret.push_back(readLine());
    }

    return ret;
}

std::string TextStream::readLineAt(Uint32 line)
{
    if (line <= 0)
        throw InvalidOperationException("A line cannot be zero indexed");

    m_position = 0;
    m_currentLine = 0;
    while (m_currentLine < line - 1)
    {
        readLine();
    }

    return readLine();
}

std::vector<std::string> TextStream::readAllLines()
{
    m_position = 0;
    m_currentLine = 0;
    std::vector<std::string> ret;

    while (!atEnd())
    {
        ret.push_back(readLine());
    }

    return ret;
}

void TextStream::setCurrentLine(Uint32 line)
{
    if (line <= 0)
        throw InvalidOperationException("A line cannot be zero indexed");
    m_currentLine = 0;
    m_position = 0;

    while(m_currentLine != line - 1)
    {
        readLine();
    }
}

Uint32 TextStream::currentLine() const
{
    return m_currentLine;
}
