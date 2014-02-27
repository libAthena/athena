#include "SpriteFileReader.hpp"
#include "SpriteFile.hpp"
#include "Sprite.hpp"
#include "SpritePart.hpp"
#include "SpriteFrame.hpp"
#include "InvalidOperationException.hpp"
#include "IOException.hpp"
#include "utility.hpp"

namespace zelda
{
namespace io
{
SpriteFileReader::SpriteFileReader(Uint8* data, Uint64 length)
    : base(data, length)
{
}

SpriteFileReader::SpriteFileReader(const std::string& filepath)
    : base(filepath)
{

}

Sakura::SpriteFile* SpriteFileReader::readFile()
{
    Uint32 magic = base::readUInt32();

    if (magic != Sakura::SpriteFile::Magic)
        throw zelda::error::InvalidOperationException("Not a valid Sakura Sprite container");

    Uint32 version = base::readUInt32();

    // TODO: Make this more verbose
    if (version != Sakura::SpriteFile::Version)
        throw zelda::error::InvalidOperationException("Unsupported version");

    // After reading in the magic and version we need to load some
    // metadata about the file.
    // Such as the texture count, it's dimensions, and it's origin.
    // After that we have the number of sprites contained in this
    // sprite container.
    Uint16 textureCount = base::readUInt16(); // Having it as a Uint16 gives us the ability to have up to 65536 different states
    // This is probably overkill, but it's better safe than sorry.
    Uint32 width        = base::readUInt32();
    Uint32 height       = base::readUInt32();
    float originX       = base::readFloat();
    float originY       = base::readFloat();
    Uint16 spriteCount  = base::readUInt16();

    // Lets go ahead and create or new container.
    Sakura::SpriteFile* ret = new Sakura::SpriteFile(width, height, originX, originY);

    // The next four bytes are reserved to keep the header 32 byte aligned.
    // This isn't necessary for most systems, but it's eventually planned
    // to migrate this code to Big Endian based systems, such as the wii
    // which require data to be 32 byte aligned, or it causes some issues.
    // It's also convenient to have this, for later expansion.
    Uint32 reserved     = base::readUInt32();
    UNUSED(reserved);

    // Next we have to load the textures
    // If we tried to add them one at a time to the sprite container
    // it will be slow as hell, so we store them in a vector locally
    // then give that vector the the container, this bypasses the de-reference
    // for each texture
#ifndef LIBZELDA_USE_QT
    std::vector<Sakura::STexture*> textures;
#else
    QList<Sakura::STexture*> textures;
#endif

    for (Uint16 i = 0; i < textureCount; i++)
    {
        Sakura::STexture* texture = new Sakura::STexture;
        texture->Filepath = base::readString();
        texture->Preload  = base::readBool();
        textures.push_back(texture);
    }

    ret->setTextures(textures);

    // Now for the sprites
    // The sprites are a bit more difficult, they are stored in an unordered_map
    // with it's name as the key, this means we can't have two sprites with the same name
    // Normally this isn't a problem, but someone may decide to copy and paste a sprite
    // and forget to change the name, that needs to be handled, but it's outside the scope
    // of this reader.
#ifndef LIBZELDA_USE_QT
    std::unordered_map <std::string, Sakura::Sprite*> sprites;
#else
    QMap<QString, Sakura::Sprite*> sprites;
#endif

    for (Uint16 i = 0; i < spriteCount; i++)
    {
        Sakura::Sprite* sprite = new Sakura::Sprite(ret);
#ifndef LIBZELDA_USE_QT
        std::string name = base::readString();
#else
        QString name = QString::fromStdString(base::readString());
#endif
        sprite->setName(name);
        Uint16 frameCount = base::readUInt16();
        Uint16 stateCount = base::readUInt16();

        // Each state id corresponds to a texture held in the parent class
        std::vector<int> stateIds;
        for (int j = 0; j < stateCount; j++)
            stateIds.push_back(base::readUInt16());


        sprite->setStateIds(stateIds);

        // Now to read the sprite parts.
        // The parts allow us to build retro style sprites very easily
        // making it possible to use one texture atlas for all possible
        // frame combinations, this reduces the amount of memory overhead
        // and the storage footprint, while Sakura supports packs and zips
        // it's still a bad idea to have a metric ton of texture resources
        // littering the place
#ifndef LIBZELDA_USE_QT
        std::vector<Sakura::SpriteFrame*> frames;
#else
        QList<Sakura::SpriteFrame*> frames;
#endif

        for (Uint32 k = 0; k < frameCount; k++)
        {
            Sakura::SpriteFrame* frame = new Sakura::SpriteFrame(sprite);
            frame->setFrameTime(base::readFloat());
            Uint16 partCount = base::readUInt16();


#ifndef LIBZELDA_USE_QT
            std::vector<Sakura::SpritePart*> parts;
#else
            QList<Sakura::SpritePart*> parts;
#endif
            for (Uint8 j = 0; j < partCount; j++)
            {
                Sakura::SpritePart* part = new Sakura::SpritePart(frame);
#ifndef LIBZELDA_USE_QT
                std::string name = base::readString();
#else
                QString name = QString::fromStdString(base::readString());
#endif
                part->setName(name);
                part->setCollision(base::readBool());

                float xOff = base::readFloat();
                float yOff = base::readFloat();
                part->setOffset(xOff, yOff);
                float texXOff = base::readFloat();
                float texYOff = base::readFloat();
                part->setTextureOffset(texXOff, texYOff);
                Uint32 width = base::readUInt32();
                Uint32 height = base::readUInt32();
                part->setSize(width, height);
                bool flippedH = base::readBool();
                part->setFlippedHorizontally(flippedH);
                bool flippedV = base::readBool();
                part->setFlippedVertically(flippedV);

                parts.push_back(part);
            }
            frame->setParts(parts);
            frames.push_back(frame);
        }

        sprite->setFrames(frames);
#ifndef LIBZELDA_USE_QT
        if (sprite->name() != std::string())
        {
            std::string nameLow(sprite->name());
            zelda::utility::tolower(nameLow);
            sprites[nameLow] = sprite;
        }
#else
        if (!sprite->name().isEmpty())
            sprites[sprite->name().toLower()] = sprite;
#endif
        else
            throw zelda::error::IOException("SSpriteFileReader::readFile -> Sprite names cannot be empty");
    }

    ret->setSprites(sprites);

    return ret;
}
} // io
} // zelda
