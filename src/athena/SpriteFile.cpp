#include "athena/SpriteFile.hpp"
#include "athena/Sprite.hpp"
#include "athena/Utility.hpp"
#include <iostream>

namespace athena::Sakura
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

SpriteFile::SpriteFile(const Vector2Di& size, const Vector2Df& origin)
    : m_size(size),
      m_origin(origin)
{
}

SpriteFile::~SpriteFile()
{
    for (std::pair<std::string, Sprite*> sprite : m_sprites)
    {
        delete sprite.second;
        sprite.second = NULL;
    }

    m_sprites.clear();
}

void SpriteFile::setSize(atUint32 width, atUint32 height)
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

atUint32 SpriteFile::width() const
{
    return m_size.x;
}

atUint32 SpriteFile::height() const
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

std::vector<STexture*> SpriteFile::textures() const
{
    return m_textures;
}

atUint32 SpriteFile::textureCount() const
{
    return (atUint32)m_textures.size();
}

void SpriteFile::addSprite(Sprite* sprite)
{
    std::string name(sprite->name());
    athena::utility::tolower(name);

    if (m_sprites.find(name) != m_sprites.end())
        return;

    m_sprites[name] = sprite;
}

void SpriteFile::removeSprite(const std::string& name)
{
    std::string tmpName(name);
    athena::utility::tolower(tmpName);
    std::unordered_map<std::string, Sprite*>::iterator iterator = m_sprites.find(tmpName);

    if (iterator != m_sprites.end())
        m_sprites.erase(iterator);
}

void SpriteFile::removeSprite(Sprite* sprite)
{
    removeSprite(sprite->name());
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
    std::string nameLow(name);
    athena::utility::tolower(nameLow);

    if (m_sprites.find(nameLow) == m_sprites.end())
        return NULL;

    return m_sprites[nameLow];
}

std::unordered_map<std::string, Sprite*> SpriteFile::sprites() const
{
    return m_sprites;
}

atUint32 SpriteFile::spriteCount() const
{
    return (atUint32)m_sprites.size();
}

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

} // zelda
