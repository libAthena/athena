#include "athena/SpritePart.hpp"
#include "athena/SpritePart.hpp"
#include "athena/Sprite.hpp"
#include <algorithm>

namespace athena
{
namespace Sakura
{

SpritePart::SpritePart(SpriteFrame* root)
    : m_root(root),
      m_hasCollision(false)
{
}

SpritePart::SpritePart(SpriteFrame* root, const std::string& name, bool hasCollision)
    : m_root(root),
      m_hasCollision(hasCollision)
{
    m_name = name;
}

SpritePart::~SpritePart()
{
}

void SpritePart::setName(const std::string& name)
{
    m_name = name;
}

std::string SpritePart::name() const
{
    return m_name;
}

void SpritePart::setCollision(bool col)
{
    m_hasCollision = col;
}

bool SpritePart::hasCollision() const
{
    return m_hasCollision;
}

void SpritePart::setOffset(float x, float y)
{
    setOffset(Vector2Df(x, y));
}

void SpritePart::setOffset(const Vector2Df& offset)
{
    m_offset = offset;
}

Vector2Df SpritePart::offset() const
{
    return m_offset;
}

void SpritePart::setTextureOffset(float x, float y)
{
    setTextureOffset(Vector2Df(x, y));
}

void SpritePart::setTextureOffset(const Vector2Df& offset)
{
    m_textureOffset = offset;
}

Vector2Df SpritePart::textureOffset() const
{
    return m_textureOffset;
}

void SpritePart::setSize(atUint32 width, atUint32 height)
{
    setSize(Vector2Di(width, height));
}

void SpritePart::setSize(const Vector2Di& size)
{
    m_size = size;
}

Vector2Di SpritePart::size() const
{
    return m_size;
}

void SpritePart::setFlippedHorizontally(const bool val)
{
    m_flippedH = val;
}

bool SpritePart::flippedHorizontally() const
{
    return m_flippedH;
}

void SpritePart::setFlippedVertically(const bool val)
{
    m_flippedV = val;
}

bool SpritePart::flippedVertically() const
{
    return m_flippedV;
}

SpriteFrame* SpritePart::root() const
{
    return m_root;
}
}
}
