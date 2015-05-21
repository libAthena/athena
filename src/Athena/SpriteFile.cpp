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

#include "Athena/SpriteFile.hpp"
#include "Athena/Sprite.hpp"
#include "Athena/Utility.hpp"
#include <iostream>

namespace Athena
{
namespace Sakura
{
const atUint32 SpriteFile::Major = 1;
const atUint32 SpriteFile::Minor = 0;
const atUint32 SpriteFile::Revision = 2;
const atUint32 SpriteFile::Build = 0;
const atUint32 SpriteFile::Version = Major | (Minor << 8) | (Revision << 16) | (Build << 24);

const atUint32 SpriteFile::Magic = 'S' | ('P' << 8) | ('R' << 16) | ('S' << 24);


SpriteFile::SpriteFile()
{
}

SpriteFile::SpriteFile(atUint32 width, atUint32 height, float originX, float originY)
    : m_size(width, height),
      m_origin(originX, originY)
{
}

#ifndef ATHENA_USE_QT
SpriteFile::SpriteFile(const Vector2Di& size, const Vector2Df& origin)
#else
SpriteFile::SpriteFile(const QSize& size, const QPoint& origin)
#endif
    : m_size(size),
      m_origin(origin)
{
}

SpriteFile::~SpriteFile()
{
#ifndef ATHENA_USE_QT

    for (std::pair<std::string, Sprite*> sprite : m_sprites)
    {
        delete sprite.second;
        sprite.second = NULL;
    }

#endif
    m_sprites.clear();
}

void SpriteFile::setSize(atUint32 width, atUint32 height)
{
#ifndef ATHENA_USE_QT
    setSize(Vector2Di(width, height));
#else
    setSize(QSize(width, height));
#endif
}

#ifndef ATHENA_USE_QT
void SpriteFile::setSize(const Vector2Di& size)
#else
void SpriteFile::setSize(const QSize& size)
#endif
{
    m_size = size;
#ifdef ATHENA_USE_QT
    emit sizeChanged(size);
#endif
}

#ifndef ATHENA_USE_QT
Vector2Di SpriteFile::size() const
#else
QSize SpriteFile::size() const
#endif
{
    return m_size;
}

atUint32 SpriteFile::width() const
{
#ifndef ATHENA_USE_QT
    return m_size.x;
#else
    return m_size.width();
#endif
}

atUint32 SpriteFile::height() const
{
#ifndef ATHENA_USE_QT
    return m_size.y;
#else
    return m_size.height();
#endif
}

void SpriteFile::setOrigin(const float x, const float y)
{
#ifndef ATHENA_USE_QT
    setOrigin(Vector2Df(x, y));
#else
    setOrigin(QPoint(x, y));
#endif
}

#ifndef ATHENA_USE_QT
void SpriteFile::setOrigin(const Vector2Df& origin)
#else
void SpriteFile::setOrigin(const QPoint& origin)
#endif
{
    m_origin = origin;
#ifdef ATHENA_USE_QT
    emit originChanged(origin);
#endif
}


#ifndef ATHENA_USE_QT
Vector2Df SpriteFile::origin() const
#else
QPoint SpriteFile::origin() const
#endif
{
    return m_origin;
}

float SpriteFile::originX() const
{
#ifndef ATHENA_USE_QT
    return m_origin.x;
#else
    return m_origin.x();
#endif
}

float SpriteFile::originY() const
{
#ifndef ATHENA_USE_QT
    return m_origin.y;
#else
    return m_origin.y();
#endif
}

bool SpriteFile::addTexture(STexture* texture)
{
    if (m_textures.size() >= 65536)
        return false;

    m_textures.push_back(texture);
    return true;
}

void SpriteFile::removeTexture(int id)
{
    if (id > (int)m_textures.size() || id < 0)
        return;

    STexture* tex = m_textures[id];
    m_textures.erase(m_textures.begin() + id);
    delete tex;
}

STexture* SpriteFile::texture(atUint32 id)
{
    if (id >= m_textures.size())
        return NULL;

    return m_textures[id];
}

#ifndef ATHENA_USE_QT
std::vector<STexture*> SpriteFile::textures() const
#else
QList<STexture*> SpriteFile::textures() const
#endif
{
    return m_textures;
}

atUint32 SpriteFile::textureCount() const
{
    return (atUint32)m_textures.size();
}

void SpriteFile::addSprite(Sprite* sprite)
{
#ifndef ATHENA_USE_QT
    std::string name(sprite->name());
    Athena::utility::tolower(name);

    if (m_sprites.find(name) != m_sprites.end())
        return;

#else
    QString name = sprite->name().toLower();

    if (m_sprites.contains(name))
        return;

#endif

    m_sprites[name] = sprite;
}

#ifndef ATHENA_USE_QT
void SpriteFile::removeSprite(const std::string& name)
{
    std::string tmpName(name);
    Athena::utility::tolower(tmpName);
    std::unordered_map<std::string, Sprite*>::iterator iterator = m_sprites.find(tmpName);

    if (iterator != m_sprites.end())
        m_sprites.erase(iterator);
}
#else
void SpriteFile::removeSprite(const QString& name)
{
    m_sprites.remove(name.toLower());
}
#endif

void SpriteFile::removeSprite(Sprite* sprite)
{
    removeSprite(sprite->name());
}

#ifndef ATHENA_USE_QT
void SpriteFile::setSprites(std::unordered_map<std::string, Sprite*> sprites)
{
    if (sprites.size() == 0)
        return;

    if (m_sprites.size() > 0)
    {
        for (std::pair<std::string, Sprite*> sprite : m_sprites)
        {
            delete sprite.second;
            sprite.second = NULL;
        }

        m_sprites.clear();
    }

    m_sprites = sprites;
}
#else
void SpriteFile::setSprites(QMap<QString, Sprite*> sprites)
{
    if (sprites.size() == 0)
        return;

    m_sprites.clear();
    m_sprites = sprites;
}
#endif

#ifndef ATHENA_USE_QT
Sprite* SpriteFile::sprite(const std::string& name)
{
    std::string nameLow(name);
    Athena::utility::tolower(nameLow);

    if (m_sprites.find(nameLow) == m_sprites.end())
        return NULL;

    return m_sprites[nameLow];
}
#else
Sprite* SpriteFile::sprite(const QString& name)
{
    if (!m_sprites.contains(name.toLower()))
        return NULL;

    return m_sprites[name.toLower()];
}
#endif

#ifndef ATHENA_USE_QT
std::unordered_map<std::string, Sprite*> SpriteFile::sprites() const
#else
QMap<QString, Sprite*> SpriteFile::sprites() const
#endif
{
    return m_sprites;
}

atUint32 SpriteFile::spriteCount() const
{
    return (atUint32)m_sprites.size();
}

#ifndef ATHENA_USE_QT
void SpriteFile::setTextures(std::vector<STexture*> textures)
{
    if (textures.size() == 0)
        return;

    if (m_textures.size() > 0)
    {
        for (STexture* tex : m_textures)
        {
            delete tex;
            tex = NULL;
        }

        m_textures.clear();
    }

    m_textures = textures;
}
#else
void SpriteFile::setTextures(QList<STexture*> textures)
{
    if (textures.size() == 0)
        return;

    m_textures.clear();
    m_textures = textures;
}
#endif
} // Sakura
} // zelda

#endif // ATHENA_NO_SAKURA
