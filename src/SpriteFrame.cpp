#include "SpriteFrame.hpp"
#include "SpritePart.hpp"

namespace zelda
{
namespace Sakura
{

SpriteFrame::SpriteFrame()
{
}

SpriteFrame::SpriteFrame(float offX, float offY, float texX, float texY, Uint32 width, Uint32 height, float frameTime, bool flippedH, bool flippedV)
    : m_offset(Vector2Df(offX, offY)),
      m_textureOffset(Vector2Df(texX, texY)),
      m_size(Vector2Di(width, height)),
      m_frameTime(frameTime),
      m_flippedH(flippedH),
      m_flippedV(flippedV)
{
}

SpriteFrame::SpriteFrame(const Vector2Df& frameOff, const Vector2Df& texOff, const Vector2Di& size, float frameTime, bool flippedH, bool flippedV)
    : m_offset(frameOff),
      m_textureOffset(texOff),
      m_size(size),
      m_frameTime(frameTime),
      m_flippedH(flippedH),
      m_flippedV(flippedV)
{
}

void SpriteFrame::setOffset(float x, float y)
{
    setOffset(Vector2Df(x, y));
}

void SpriteFrame::setOffset(const Vector2Df& offset)
{
    m_offset = offset;
}

Vector2Df SpriteFrame::offset() const
{
    return m_offset;
}

void SpriteFrame::setTextureOffset(float x, float y)
{
    setTextureOffset(Vector2Df(x, y));
}

void SpriteFrame::setTextureOffset(const Vector2Df& texOff)
{
    m_textureOffset = texOff;
}

Vector2Df SpriteFrame::textureOffset() const
{
    return m_textureOffset;
}

void SpriteFrame::setSize(Uint32 width, Uint32 height)
{
    setSize(Vector2Di(width, height));
}

void SpriteFrame::setSize(const Vector2Di& size)
{
    m_size = size;
}

Vector2Di SpriteFrame::size() const
{
    return m_size;
}

void SpriteFrame::setFlippedHorizontally(const bool val)
{
    m_flippedH = val;
}

bool SpriteFrame::flippedHorizontally() const
{
    return m_flippedH;
}

void SpriteFrame::setFlippedVertically(const bool val)
{
    m_flippedV = val;
}

bool SpriteFrame::flippedVertically() const
{
    return m_flippedV;
}

void SpriteFrame::setFrameTime(float frameTime)
{
    m_frameTime = frameTime;
}

float SpriteFrame::frameTime() const
{
    return m_frameTime;
}

} // Sakura
} // zelda
