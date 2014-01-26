#include "SpritePart.hpp"
#include "SpriteFrame.hpp"
#include "Sprite.hpp"

namespace zelda
{
namespace Sakura
{

SpritePart::SpritePart(Sprite* root)
    : m_root(root),
      m_hasCollision(false),
      m_currentFrame(NULL),
      m_frameIndex(0)
{
}

SpritePart::SpritePart(Sprite* root, const std::string& name, bool hasCollision)
    : m_root(root),
      m_name(name),
      m_hasCollision(hasCollision),
      m_currentFrame(NULL),
      m_frameIndex(0)
{
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

void SpritePart::addFrame(SpriteFrame* frame)
{
    m_frames.push_back(frame);
}

void SpritePart::advanceFrame()
{
    if (m_frameIndex < m_frames.size() - 1)
    {
        m_frameIndex++;
        m_currentFrame = m_frames[m_frameIndex];
    }
}

void SpritePart::retreatFrame()
{
    if (m_frameIndex > 0)
    {
        m_frameIndex--;
        m_currentFrame = m_frames[m_frameIndex];
    }
}

SpriteFrame* SpritePart::frame(int id)
{
    if (id < 0 || id >= (int)m_frames.size())
        return NULL;

    return m_frames[id];
}

void SpritePart::setFrames(std::vector<SpriteFrame*> frames)
{
    if (frames.size() == 0)
        return;

    if (m_frames.size() > 0)
    {
        for (SpriteFrame* frame : m_frames)
        {
            delete frame;
            frame = NULL;
        }
        m_frames.clear();
    }

    if (!m_currentFrame)
    {
        m_currentFrame = frames[0];
        updateTexture();
    }

    m_frames = frames;
}

SpriteFrame* SpritePart::currentFrame()
{
    return m_currentFrame;
}

int SpritePart::currentFrameID()
{
    return m_frameIndex;
}

std::vector<SpriteFrame*> SpritePart::frames() const
{
    return m_frames;
}

Uint32 SpritePart::frameCount() const
{
    return m_frames.size();
}

}
}
