#ifndef __TEXTSTREAM_HPP__
#define __TEXTSTREAM_HPP__

#include "Stream.hpp"
#include <string>
#include <vector>


// TODO (Phil#1#): Need to actually use AccessMode
/*! \class TextStream
 *  \brief A Class for reading or writing Text data.
 *
 *  TextStream is a class for reading/writing TextData.<br />
 *  It is currently incomplete, but usable, so use with caution.<br/>
 *  Since it's incomplete it may not behave exactly as expected.
 */
class TextStream : public Stream
{
public:
    /*! \enum TextMode
     *  \brief Specifies how the file is opened.
     */
    enum TextMode
    {
        Open,                       //!< The file is opened if it exists.
        Create,                     //!< Create the file if it does not exist.
        OpenOrCreate = Open|Create, //!< If the file does not exist when opening the file it is created
        Truncate,                   //!< All the data currently that is in the file is erased.
        Append                      //!< After opening the file the current line is set to the end of the buffer
    };

    /*! \enum AccessMode
     *  \brief Specifies how the Stream can be interacted with.
     */
    enum AccessMode
    {
        ReadOnly, //!< The Stream can only be read from.
        WriteOnly,//!< The Stream can only be written to.
        ReadWrite //!< The Stream can be read from or written to.
    };

    /*! \brief This constructor opens the file and loads all the lines. */
    TextStream(const std::string& filename, TextMode fileMode = Open, AccessMode accessMode = ReadWrite);

    /*! \brief Creates a new buffer and saves all lines to the specified file.
     *  \param filename The file, including path to save to.
     */
    void save(const std::string& filename = "");

    /*! \brief Reads the line at the current address.
     *
     *  \return std::string The line read.
     */
    std::string readLine();

    /*! \brief Writes a line to the buffer
     *
     *  \param str The string to write.
     */
    void writeLine(const std::string& str);

    /*! \brief Reads a given amount of lines relative to the current address
     *
     *  \param numLines The amount of lines to read.
     *  \return std::vector<std::string> The lines read.
     */
    std::vector<std::string> readLines(Uint32 numLines);

    /*! \brief Reads a given list of lines relative to the current address
     *
     *  \param lines The lines to write.
     */
    void writeLines(std::vector<std::string> lines);

    /*! \brief Reads all the lines in the current buffer.
     *
     *  \return The lines read.
     */
    std::vector<std::string> readAllLines();

    /*! \brief Reads a line at the given address.
     *
     *  \param line The line to read.
     *  \return std::string The lines read.
     */
    std::string readLineAt(Uint32 line);

    /*! \brief Writes a line at the given address.
     *
     *  \param line The address to write to.
     *  \param str The string to write.
     */
    void writeLineAt(Uint32 line, const std::string& str);

    /*! \brief Sets the current line in the Stream.
     *
     *  \param line The line to seek to.
     */
    void setCurrentLine(Uint32 line);

    /*! \brief Returns the current line in the stream.
     *
     *  \return Uint32 The current line in the stream.
     */
    Uint32 currentLine() const;

    /*! \brief Sets the AccessMode of the Stream.
     *
     *  \param mode The mode to set.
     */
     void setAccessMode(AccessMode mode);

    /*! \brief Returns the AccessMode of the Stream.
     *
     *  \return AccessModeThe mode to set.
     */
     AccessMode accessMode() const;

     /*! \brief Sets the Textmode of the Stream.
     *
     *  \param mode The mode to set.
     */
     void setTextMode(TextMode mode);

    /*! \brief Returns the TextMode of the Stream.
     *
     *  \return TextMode The mode to set.
     */
     TextMode textMode() const;

     bool isOpenForReading() const;
     bool isOpenForWriting() const;
private:
    void        loadLines();
    std::string m_filename;
    TextMode    m_textmode;
    AccessMode  m_accessmode;

    std::vector<std::string> m_lines;
    Uint32      m_currentLine;
    Uint32      m_startLength;
};

#endif
