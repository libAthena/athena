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

    for (std::pair<std::string, Sakura::Sprite*> spritePair : file->sprites())
    {
        Sakura::Sprite* sprite = spritePair.second;

        base::writeString(sprite->name());
        base::writeUInt16(sprite->partCount());
        base::writeUInt16(sprite->stateCount());

        for (int id : sprite->stateIds())
            base::writeUInt16(id);

        for (Sakura::SpritePart* part : sprite->parts())
        {
            base::writeString(part->name());
            base::writeBool(part->hasCollision());
            base::writeUInt32(part->frameCount());

            for (Sakura::SpriteFrame* frame : part->frames())
            {
                base::writeFloat(frame->offset().x);
                base::writeFloat(frame->offset().y);
                base::writeFloat(frame->textureOffset().x);
                base::writeFloat(frame->textureOffset().y);
                base::writeUInt32(frame->size().x);
                base::writeUInt32(frame->size().y);
                base::writeFloat(frame->frameTime());
                base::writeBool(frame->flippedHorizontally());
                base::writeBool(frame->flippedVertically());
            }
        }
    }

    save();
}

} // io
} // zelda
