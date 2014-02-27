#include "SpriteFileWriter.hpp"
#include "SpriteFile.hpp"
#include "Sprite.hpp"
#include "SpritePart.hpp"
#include "SpriteFrame.hpp"
#include "InvalidOperationException.hpp"

namespace zelda
{
namespace io
{
SpriteFileWriter::SpriteFileWriter(Uint8 *data, Uint64 length)
    : base(data, length)
{
}

SpriteFileWriter::SpriteFileWriter(const std::string& filepath)
    : base(filepath)
{
}

void SpriteFileWriter::writeFile(Sakura::SpriteFile* file)
{
    if (!file)
        throw zelda::error::InvalidOperationException("SSpriteFileWriter::writeFile -> file cannot be NULL");

    base::writeUInt32(Sakura::SpriteFile::Magic);
    base::writeUInt32(Sakura::SpriteFile::Version);

    base::writeUInt16(file->textureCount());
    base::writeUInt32(file->width());
    base::writeUInt32(file->height());
    base::writeFloat(file->originX());
    base::writeFloat(file->originY());
    base::writeUInt16(file->spriteCount());

    base::writeUInt32(0xFFFFFFFF);

    for (Sakura::STexture* texture : file->textures())
    {
        base::writeString(texture->Filepath);
        base::writeBool(texture->Preload);
    }

#ifndef LIBZELDA_USE_QT
    for (std::pair<std::string, Sakura::Sprite*> spritePair : file->sprites())
    {
        Sakura::Sprite* sprite = spritePair.second;
        base::writeString(sprite->name());
#else
    foreach(Sakura::Sprite* sprite, file->sprites().values())
    {

        base::writeString(sprite->name().toStdString());
#endif
        base::writeUInt16(sprite->frameCount());
        base::writeUInt16(sprite->stateCount());

        for (int id : sprite->stateIds())
            base::writeUInt16(id);

        for (Sakura::SpriteFrame* frame : sprite->frames())
        {
            base::writeFloat(frame->frameTime());
            base::writeUInt16(frame->partCount());
            for (Sakura::SpritePart* part: frame->parts())
            {
#ifndef LIBZELDA_USE_QT
                base::writeString(part->name());
#else
                base::writeString(part->name().toStdString());
#endif
                base::writeBool(part->hasCollision());
#ifndef LIBZELDA_USE_QT
                base::writeFloat(part->offset().x);
                base::writeFloat(part->offset().y);
                base::writeFloat(part->textureOffset().x);
                base::writeFloat(part->textureOffset().y);
                base::writeUInt32(part->size().x);
                base::writeUInt32(part->size().y);
#else
                base::writeFloat(part->offset().x());
                base::writeFloat(part->offset().y());
                base::writeFloat(part->textureOffset().x());
                base::writeFloat(part->textureOffset().y());
                base::writeUInt32(part->size().width());
                base::writeUInt32(part->size().height());
#endif
                base::writeBool(part->flippedHorizontally());
                base::writeBool(part->flippedVertically());
            }
        }
    }

    save();
}

} // io
} // zelda
