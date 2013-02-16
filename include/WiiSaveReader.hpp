#ifndef __WII_SAVE_READER_HPP__
#define __WII_SAVE_READER_HPP__

#include <Types.hpp>
#include <utility.hpp>
#include <BinaryReader.hpp>

class WiiSave;
class WiiBanner;
class WiiFile;
class WiiImage;

class WiiSaveReader : public BinaryReader
{
public:
    WiiSaveReader(const Uint8*, Uint64);
    WiiSaveReader(const std::string&);

    WiiSave* readSave();
private:
    WiiBanner* readBanner();
    WiiFile*   readFile();
    WiiImage*  readImage(Uint32 width, Uint32 height);
    void       readCerts(Uint32 totalSize);
};
#endif // __WII_SAVE_READER_HPP__
