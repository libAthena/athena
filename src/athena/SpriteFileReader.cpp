#include "athena/SpriteFileReader.hpp"
#include "athena/SpriteFile.hpp"
#include "athena/Sprite.hpp"
#include "athena/SpritePart.hpp"
#include "athena/SpriteFrame.hpp"
#include "athena/Utility.hpp"

namespace athena::io
{
SpriteFileReader::SpriteFileReader(atUint8* data, atUint64 length)
    : MemoryCopyReader(data, length)
{
}

SpriteFileReader::SpriteFileReader(const std::string& filepath)
    : MemoryCopyReader(filepath)
{

}

Sakura::SpriteFile* SpriteFileReader::readFile()
{
    Sakura::SpriteFile* ret = NULL;

    atUint32 magic = readUint32();

    if (magic != Sakura::SpriteFile::Magic)
    {
        atError("Not a valid Sakura Sprite container");
        return nullptr;
    }

    atUint32 version = readUint32();

    // TODO: Make this more verbose
    if (version != Sakura::SpriteFile::Version)
    {
        atError("Unsupported version");
        return nullptr;
    }

    // After reading in the magic and version we need to load some
    // metadata about the file.
    // Such as the texture count, it's dimensions, and it's origin.
    // After that we have the number of sprites contained in this
    // sprite container.
    atUint16 textureCount = readUint16(); // Having it as a Uint16 gives us the ability to have up to 65536 different states
    // This is probably overkill, but it's better safe than sorry.
    atUint32 width        = readUint32();
    atUint32 height       = readUint32();
    float originX       = readFloat();
    float originY       = readFloat();
    atUint16 spriteCount  = readUint16();

    // Lets go ahead and create or new container.
    ret = new Sakura::SpriteFile(width, height, originX, originY);

    // The next four bytes are reserved to keep the header 32 byte aligned.
    // This isn't necessary for most systems, but it's eventually planned
    // to migrate this code to Big Endian based systems, such as the wii
    // which require data to be 32 byte aligned, or it causes some issues.
    // It's also convenient to have this, for later expansion.
    atUint32 reserved     = readUint32();
    UNUSED(reserved);

    // Next we have to load the textures
    // If we tried to add them one at a time to the sprite container
    // it will be slow as hell, so we store them in a vector locally
    // then give that vector the the container, this bypasses the de-reference
    // for each texture
#ifndef ATHENA_USE_QT
    std::vector<Sakura::STexture*> textures;
#else
    QList<Sakura::STexture*> textures;
#endif

    for (atUint16 i = 0; i < textureCount; i++)
    {
        Sakura::STexture* texture = new Sakura::STexture;
        texture->Filepath = readString();
        texture->Preload  = readBool();
        textures.push_back(texture);
    }

    ret->setTextures(textures);

    // Now for the sprites
    // The sprites are a bit more difficult, they are stored in an unordered_map
    // with it's name as the key, this means we can't have two sprites with the same name
    // Normally this isn't a problem, but someone may decide to copy and paste a sprite
    // and forget to change the name, that needs to be handled, but it's outside the scope
    // of this reader.
#ifndef ATHENA_USE_QT
    std::unordered_map <std::string, Sakura::Sprite*> sprites;
#else
    QMap<QString, Sakura::Sprite*> sprites;
#endif

    for (atUint16 i = 0; i < spriteCount; i++)
    {
        Sakura::Sprite* sprite = new Sakura::Sprite(ret);
#ifndef ATHENA_USE_QT
        std::string name = readString();
#else
        QString name = QString::fromStdString(readString());
#endif
        sprite->setName(name);
        atUint16 frameCount = readUint16();
        atUint16 stateCount = readUint16();

        // Each state id corresponds to a texture held in the parent class
        std::vector<int> stateIds;

        for (int j = 0; j < stateCount; j++)
            stateIds.push_back(readUint16());


        sprite->setStateIds(stateIds);

        // Now to read the sprite parts.
        // The parts allow us to build retro style sprites very easily
        // making it possible to use one texture atlas for all possible
        // frame combinations, this reduces the amount of memory overhead
        // and the storage footprint, while Sakura supports packs and zips
        // it's still a bad idea to have a metric ton of texture resources
        // littering the place
#ifndef ATHENA_USE_QT
        std::vector<Sakura::SpriteFrame*> frames;
#else
        QList<Sakura::SpriteFrame*> frames;
#endif

        for (atUint32 k = 0; k < frameCount; k++)
        {
            Sakura::SpriteFrame* frame = new Sakura::SpriteFrame(sprite);
            frame->setFrameTime(readFloat());
            atUint16 partCount = readUint16();


#ifndef ATHENA_USE_QT
            std::vector<Sakura::SpritePart*> parts;
#else
            QList<Sakura::SpritePart*> parts;
#endif

            for (atUint8 j = 0; j < partCount; j++)
            {
                Sakura::SpritePart* part = new Sakura::SpritePart(frame);
#ifndef ATHENA_USE_QT
                std::string name = readString();
#else
                QString name = QString::fromStdString(readString());
#endif
                part->setName(name);
                part->setCollision(readBool());

                float xOff = readFloat();
                float yOff = readFloat();
                part->setOffset(xOff, yOff);
                float texXOff = readFloat();
                float texYOff = readFloat();
                part->setTextureOffset(texXOff, texYOff);
                atUint32 width = readUint32();
                atUint32 height = readUint32();
                part->setSize(width, height);
                bool flippedH = readBool();
                part->setFlippedHorizontally(flippedH);
                bool flippedV = readBool();
                part->setFlippedVertically(flippedV);

                parts.push_back(part);
            }

            frame->setParts(parts);
            frames.push_back(frame);
        }

        sprite->setFrames(frames);
#ifndef ATHENA_USE_QT

        if (sprite->name() != std::string())
        {
            std::string nameLow(sprite->name());
            athena::utility::tolower(nameLow);
            sprites[nameLow] = sprite;
        }

#else

        if (!sprite->name().isEmpty())
            sprites[sprite->name().toLower()] = sprite;

#endif
        else
        {
            atError("Sprite names cannot be empty");
            return nullptr;
        }
    }

    ret->setSprites(sprites);

    return ret;
}
} // zelda
