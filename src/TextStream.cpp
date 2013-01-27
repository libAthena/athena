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

    loadLines();
}

void  TextStream::save(const std::string& filename)
{
    if (filename != std::string())
        m_filename = filename;

    // We need a new buffer to write the new lines
    if (m_data)
        delete[] m_data;

    m_position = 0;
    m_length = 1;

    // Set the new buffer
    m_data = new Uint8[m_length];

    // Now write all the strings to the new buffer
    for (std::string s : m_lines)
        writeBytes((Int8*)s.c_str(), s.size());

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
    if (m_currentLine > m_lines.size())
        throw IOException("Position past stream bounds");
    return m_lines[m_currentLine++];
}


void  TextStream::writeLine(const std::string& str)
{
    if (m_currentLine > m_lines.size() && !m_autoResize)
        throw IOException("Position past stream bounds");
    else if (m_currentLine > m_lines.size())
    {
        m_lines.push_back(str);
        m_currentLine++;
        return;
    }

    m_lines[m_currentLine++] = str;
}

void TextStream::writeLines(std::vector<std::string> strings)
{
    for (std::string s: strings)
        writeLine(s);
}

std::vector<std::string> TextStream::readLines(Uint32 numLines)
{
    Uint32 currentLine = m_currentLine;
    std::vector<std::string> ret;

    while ((m_currentLine++) <= currentLine + numLines)
        ret.push_back(m_lines[m_currentLine]);

    return ret;
}

std::string TextStream::readLineAt(Uint32 line)
{
    if (line <= 0)
        throw InvalidOperationException("A line cannot be zero indexed");

    return m_lines[line - 1];
}

std::vector<std::string> TextStream::readAllLines()
{
    return m_lines;
}

void TextStream::setCurrentLine(Uint32 line)
{
    if (line <= 0)
        throw InvalidOperationException("A line cannot be zero indexed");
    m_currentLine = line - 1;
}

Uint32 TextStream::currentLine() const
{
    return m_currentLine + 1;
}

void TextStream::loadLines()
{
    while (!atEnd())
    {
        std::string line;
        Uint8 c;
        for (;;)
        {
            c = readByte();

            if (c == '\r' || c == '\n')
            {
                m_currentLine++;
                line.push_back(c);
                if (*(Uint8*)(m_data + m_position + 1) == '\n')
                {
                    line.push_back('\n');
                    m_position++; // advance position past the new line character
                }
                break;
            }

            if (c == '\0')
            {
                line.push_back('\n');
                break;
            }
            line.push_back(c);
        }

        m_lines.push_back(line);
    }
}
