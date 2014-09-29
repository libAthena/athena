#ifndef ATHENA_NO_SAKURA
// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

#include "Athena/SpriteFileWriter.hpp"
#include "Athena/SpriteFile.hpp"
#include "Athena/Sprite.hpp"
#include "Athena/SpritePart.hpp"
#include "Athena/SpriteFrame.hpp"
#include "Athena/InvalidOperationException.hpp"
#include "Athena/InvalidDataException.hpp"

namespace Athena
{
namespace io
{
SpriteFileWriter::SpriteFileWriter(atUint8 *data, atUint64 length)
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
        THROW_INVALID_OPERATION_EXCEPTION("SSpriteFileWriter::writeFile -> file cannot be NULL");

    base::writeUint32(Sakura::SpriteFile::Magic);
    base::writeUint32(Sakura::SpriteFile::Version);

    base::writeUint16(file->textureCount());
    base::writeUint32(file->width());
    base::writeUint32(file->height());
    base::writeFloat(file->originX());
    base::writeFloat(file->originY());
    base::writeUint16(file->spriteCount());

    base::writeUint32(0xFFFFFFFF);

    for (Sakura::STexture* texture : file->textures())
    {
        base::writeString(texture->Filepath);
        base::writeBool(texture->Preload);
    }

#ifndef ATHENA_USE_QT
    for (std::pair<std::string, Sakura::Sprite*> spritePair : file->sprites())
    {
        Sakura::Sprite* sprite = spritePair.second;
        base::writeString(sprite->name());
#else
    foreach(Sakura::Sprite* sprite, file->sprites().values())
    {

        base::writeString(sprite->name().toStdString());
#endif
        base::writeUint16(sprite->frameCount());
        base::writeUint16(sprite->stateCount());

        for (int id : sprite->stateIds())
            base::writeUint16(id);

        for (Sakura::SpriteFrame* frame : sprite->frames())
        {
            base::writeFloat(frame->frameTime());
            base::writeUint16(frame->partCount());
            for (Sakura::SpritePart* part: frame->parts())
            {
#ifndef ATHENA_USE_QT
                base::writeString(part->name());
#else
                base::writeString(part->name().toStdString());
#endif
                base::writeBool(part->hasCollision());
#ifndef ATHENA_USE_QT
                base::writeFloat(part->offset().x);
                base::writeFloat(part->offset().y);
                base::writeFloat(part->textureOffset().x);
                base::writeFloat(part->textureOffset().y);
                base::writeUint32(part->size().x);
                base::writeUint32(part->size().y);
#else
                base::writeFloat(part->offset().x());
                base::writeFloat(part->offset().y());
                base::writeFloat(part->textureOffset().x());
                base::writeFloat(part->textureOffset().y());
                base::writeUint32(part->size().width());
                base::writeUint32(part->size().height());
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
#endif // ATHENA_NO_SAKURA
