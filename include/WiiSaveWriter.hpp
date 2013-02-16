#ifndef __WII_SAVE_WRITER_HPP__
#define __WII_SAVE_WRITER_HPP__

#include <Types.hpp>
#include <utility.hpp>
#include <BinaryWriter.hpp>

class WiiSave;
class WiiBanner;
class WiiFile;
class WiiImage;

class WiiSaveWriter : public BinaryWriter
{
public:
    WiiSaveWriter(const std::string&);

    bool writeSave(WiiSave* save, Uint8* macAddress, Uint32 ngId, Uint8* ngPriv, Uint8* ngSig, Uint32 ngKeyId, const std::string& filepath = "");

private:
    void writeBanner(WiiBanner* banner);
    Uint32 writeFile(WiiFile* file);
    void writeImage(WiiImage* image);
    void writeCerts(Uint32 filesSize, Uint32 ngId, Uint8* ngPriv, Uint8* ngSig, Uint32 ngKeyId);
};

#endif // __WII_SAVE_WRITER_HPP__
