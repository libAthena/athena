#include "SpriteFile.hpp"
#include "Sprite.hpp"
#include <iostream>


namespace zelda
{
namespace Sakura
{
const Uint32 SpriteFile::Major = 1;
const Uint32 SpriteFile::Minor = 0;
const Uint32 SpriteFile::Revision = 1;
const Uint32 SpriteFile::Build = 0;
const Uint32 SpriteFile::Version = Major | (Minor << 8) | (Revision << 16) | (Build << 24);

const Uint32 SpriteFile::Magic = 'S' | ('P' << 8) | ('R' << 16) | ('S' << 24);


SpriteFile::SpriteFile()
    : m_size(Vector2Di(1, 1))
{
}

SpriteFile::SpriteFile(Uint32 width, Uint32 height, float originX, float originY)
    : m_size(Vector2Di(width, height)),
      m_origin(Vector2Df(originX, originY))
{
}

SpriteFile::SpriteFile(const Vector2Di& size, const Vector2Df& origin)
    : m_size(size),
      m_origin(origin)
{
}

void SpriteFile::setSize(Uint32 width, Uint32 height)
{
    setSize(Vector2Di(width, height));
}

void SpriteFile::setSize(const Vector2Di& size)
{
    m_size = size;
}

Vector2Di SpriteFile::size() const
{
    return m_size;
}

Uint32 SpriteFile::width() const
{
    return m_size.x;
}

Uint32 SpriteFile::height() const
{
    return m_size.y;
}

void SpriteFile::setOrigin(const float x, const float y)
{
    setOrigin(Vector2Df(x, y));
}

void SpriteFile::setOrigin(const Vector2Df& origin)
{
    m_origin = origin;
}

Vector2Df SpriteFile::origin() const
{
    return m_origin;
}

float SpriteFile::originX() const
{
    return m_origin.x;
}

float SpriteFile::originY() const
{
    return m_origin.y;
}

void SpriteFile::addTexture(STexture* texture)
{
    m_textures.push_back(texture);
}

void SpriteFile::removeTexture(int id)
{
    if (id > (int)m_textures.size() || id < 0)
        return;

    STexture* tex = m_textures[id];
    m_textures.erase(m_textures.begin() + id);
    delete tex;
}

STexture* SpriteFile::texture(Uint32 id)
{
    if (id >= m_textures.size())
        return NULL;

    return m_textures[id];
}

std::vector<STexture*> SpriteFile::textures() const
{
    return m_textures;
}

Uint32 SpriteFile::textureCount() const
{
    return m_textures.size();
}

void SpriteFile::addSprite(Sprite* sprite)
{
    if (m_sprites.find(sprite->name()) != m_sprites.end())
        return;

    m_sprites[sprite->name()] = sprite;
}

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

Sprite* SpriteFile::sprite(const std::string& name)
{
    if (m_sprites.find(name) == m_sprites.end())
        return NULL;

    return m_sprites[name];
}

std::unordered_map<std::string, Sprite*> SpriteFile::sprites() const
{
    return m_sprites;
}

Uint32 SpriteFile::spriteCount() const
{
    return m_sprites.size();
}

void SpriteFile::setTextures(std::vector<STexture*> textures)
{
    if (textures.size() == 0)
        return;

    if (m_textures.size() > 0)
    {
        for(STexture* tex : m_textures)
        {
            delete tex;
            tex = NULL;
        }
        m_textures.clear();
    }

    m_textures = textures;
}
} // Sakura
} // zelda
