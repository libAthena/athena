#ifndef WIISAVE_H
#define WIISAVE_H

#include <unordered_map>
#include <string>
#include <Types.hpp>

class WiiFile;
class WiiBanner;
class WiiImage;
class BinaryReader;
class BinaryWriter;

class WiiSave
{
public:
    typedef std::unordered_map<std::string, WiiFile*>::const_iterator FileIterator;
    WiiSave();
    virtual ~WiiSave();

    void     addFile(const std::string& filename, WiiFile* file);
    WiiFile* file(const std::string& filename) const;
    std::unordered_map<std::string, WiiFile*>& fileList();

    void     setBanner(WiiBanner* banner);
    WiiBanner* banner() const;

protected:
private:

    void       writeBanner();
    Uint32     writeFile(WiiFile* file);
    void       writeImage(WiiImage* image);
    void       writeCerts(Uint32 filesSize, Uint32 ngId, Uint8* ngPriv, Uint8* ngSig, Uint32 ngKeyId);

    std::string readNullTermString();

    std::unordered_map<std::string, WiiFile*> m_files;
    WiiBanner* m_banner;

    BinaryReader* m_reader;
    BinaryWriter* m_writer;
};

#endif // WIISAVE_H
