#include "athena/SpriteFrame.hpp"
#include "athena/SpritePart.hpp"
#include "athena/Sprite.hpp"

namespace athena
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
#ifdef ATHENA_USE_QT
    emit frameTimeChanged(frameTime);
#endif
}

#ifndef ATHENA_USE_QT
void SpriteFrame::setParts(std::vector<SpritePart*> parts)
#else
void SpriteFrame::setParts(QList<SpritePart*> parts)
#endif
{
    m_parts = parts;
}

#ifndef ATHENA_USE_QT
std::vector<SpritePart*> SpriteFrame::parts() const
#else
QList<SpritePart*> SpriteFrame::parts() const
#endif
{
    return m_parts;
}

atUint32 SpriteFrame::partCount() const
{
    return (atUint32)m_parts.size();
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
