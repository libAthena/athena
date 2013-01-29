#ifndef __MAINPAGE_HPP__
#define __MAINPAGE_HPP__

/*! \mainpage libZelda
 *  \section intro_sec Introduction to libZelda
 *  libZelda is an IO Library which is designed to be powerful, simple,
 *  and infinitely extensible. <br />
 *  <br />
 *  libZelda provides several basic classes that can be used to read from
 *  and write to files, and memory, classes such as Stream, BinaryReader, BinaryWriter,
 *  and the currently work in progress TextStream.
 *  \section example_sec BinaryWriter example
 *  \code
 *  #include "BinaryWriter.hpp"
 *  #include "FileNotFoundException.hpp"
 *  #include "Exception.hpp"
 *  int main()
 *  {
 *       try
 *       {
 *           BinaryWriter writer("test.bin");
 *           writer.writeByte('t');
 *           writer.writeInt32(123454321);
 *           writer.save();
 *       }
 *       catch (FileNotFoundException e)
 *       {
 *           std::cout << e.message() << std::endl;
 *       }
 *       catch (Exception e)
 *       {
 *           std::cout << e.message() << std::endl;
 *       }
 *       catch(...)
 *       {
 *       }
 *       return 0;
 *  }
 *  \endcode
 *  \section example_sec BinaryReader example
 *  \code
 *  #include "BinaryReader.hpp"
 *  #include "FileNotFoundException.hpp"
 *  #include "Exception.hpp"
 *  int main()
 *  {
 *       try
 *       {
 *           BinaryReader writer("test.bin");
 *           std::cout << reader.readByte() << std::endl;
 *           std::cout << reader.readInt32() << std::endl;
 *       }
 *       catch (FileNotFoundException e)
 *       {
 *           std::cout << e.message() << std::endl;
 *       }
 *       catch (Exception e)
 *       {
 *           std::cout << e.message() << std::endl;
 *       }
 *       catch(...)
 *       {
 *       }
 *       return 0;
 *  }
 *  \endcode
 *  \section Credits
 *  Chibi Zelda:  <a href="http://animewaterfall.deviantart.com/art/Chibi-Zelda-331611090">AnimeWaterFall</a> on Deviantart
 */
#endif // __MAINPAGE_HPP__
