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

    std::unordered_map<std::string, WiiFile*> m_files;
    WiiBanner* m_banner;

};

#endif // WIISAVE_H
