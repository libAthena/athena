#include "athena/SpriteFileWriter.hpp"
#include "athena/SpriteFile.hpp"
#include "athena/Sprite.hpp"
#include "athena/SpritePart.hpp"
#include "athena/SpriteFrame.hpp"

namespace athena::io
{
SpriteFileWriter::SpriteFileWriter(atUint8* data, atUint64 length)
    : MemoryCopyWriter(data, length)
{
}

SpriteFileWriter::SpriteFileWriter(std::string_view filepath)
    : MemoryCopyWriter(filepath)
{
}

void SpriteFileWriter::writeFile(Sakura::SpriteFile* file)
{
    if (!file)
    {
        atError("file cannot be NULL");
        return;
    }

    writeUint32(Sakura::SpriteFile::Magic);
    writeUint32(Sakura::SpriteFile::Version);

    writeUint16(file->textureCount());
    writeUint32(file->width());
    writeUint32(file->height());
    writeFloat(file->originX());
    writeFloat(file->originY());
    writeUint16(file->spriteCount());

    writeUint32(0xFFFFFFFF);

    for (Sakura::STexture* texture : file->textures())
    {
        writeString(texture->Filepath);
        writeBool(texture->Preload);
    }

    for (std::pair<std::string, Sakura::Sprite*> spritePair : file->sprites())
    {
        Sakura::Sprite* sprite = spritePair.second;
        writeString(sprite->name());
        writeUint16(sprite->frameCount());
        writeUint16(sprite->stateCount());

        for (int id : sprite->stateIds())
            writeUint16(id);

        for (Sakura::SpriteFrame* frame : sprite->frames())
        {
            writeFloat(frame->frameTime());
            writeUint16(frame->partCount());

            for (Sakura::SpritePart* part : frame->parts())
            {
                writeString(part->name());
                writeBool(part->hasCollision());
                writeFloat(part->offset().x);
                writeFloat(part->offset().y);
                writeFloat(part->textureOffset().x);
                writeFloat(part->textureOffset().y);
                writeUint32(part->size().x);
                writeUint32(part->size().y);
                writeBool(part->flippedHorizontally());
                writeBool(part->flippedVertically());
            }
        }
    }

    save();
}

} // zelda
