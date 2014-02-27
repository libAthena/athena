#include "SpriteFrame.hpp"
#include "SpritePart.hpp"
#include "Sprite.hpp"

namespace zelda
{
namespace Sakura
{

SpriteFrame::SpriteFrame()
{
}

SpriteFrame::SpriteFrame(Sprite* root)
    : m_root(root)
{
}

void SpriteFrame::setFrameTime(float frameTime)
{
    m_frameTime = frameTime;
#ifdef LIBZELDA_USE_QT
    emit frameTimeChanged(frameTime);
#endif
}

#ifndef LIBZELDA_USE_QT
void SpriteFrame::setParts(std::vector<SpritePart*> parts)
#else
void SpriteFrame::setParts(QList<SpritePart*> parts)
#endif
{
    m_parts = parts;
}

#ifndef LIBZELDA_USE_QT
std::vector<SpritePart*> SpriteFrame::parts() const
#else
QList<SpritePart*> SpriteFrame::parts() const
#endif
{
    return m_parts;
}

Uint32 SpriteFrame::partCount() const
{
    return m_parts.size();
}

float SpriteFrame::frameTime() const
{
    return m_frameTime;
}

void SpriteFrame::setRoot(Sprite* root)
{
    root->removeFrame(this);
    m_root = root;
}

Sprite* SpriteFrame::root() const
{
    return m_root;
}

} // Sakura
} // zelda
