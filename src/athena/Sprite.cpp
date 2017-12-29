#include "athena/Sprite.hpp"
#include "athena/SpriteFrame.hpp"
#include "athena/SpriteFile.hpp"
#include <algorithm>

#ifdef ATHENA_USE_QT
#include <QVector>
#endif

namespace athena::Sakura
{
Sprite::Sprite(SpriteFile* root)
    : m_root(root),
      m_currentState(0),
      m_currentFrame(0)
{
}

Sprite::Sprite(SpriteFile* root, const std::string& name)
    : m_root(root),
      m_currentState(0)
{
    m_name = name;
}

Sprite::~Sprite()
{
    for (SpriteFrame* frame : m_frames)
    {
        delete frame;
        frame = NULL;
    }

    m_frames.clear();
}

void Sprite::setPosition(const float x, const float y)
{
    setPosition(Vector2Df(x, y));
}

void Sprite::setPosition(const Vector2Df& pos)
{
    m_position = pos;
}

Vector2Df Sprite::position() const
{
    return m_position;
}

void Sprite::setName(const std::string& name)
{
    m_name = name;
}

std::string Sprite::name() const
{
    return m_name;
}

void Sprite::addStateId(int id)
{
    if (m_stateIds.size() >= 65536)
        return;

    if (std::find(m_stateIds.begin(), m_stateIds.end(), id) == m_stateIds.end())
        m_stateIds.push_back(id);
}

int Sprite::stateId(int index) const
{
    if (index >= (int)m_stateIds.size())
        return -1;

    return m_stateIds[index];
}

void Sprite::setStateIds(std::vector<int> ids)
{
    if (ids.size() == 0)
        return;

    m_stateIds = ids;
}

std::vector<int> Sprite::stateIds() const
{
    return m_stateIds;
}

atUint32 Sprite::stateCount() const
{
    return (atUint32)m_stateIds.size();
}

void Sprite::setCurrentState(const atUint32 id)
{
    if (id >= m_stateIds.size())
        return;

    m_currentState = id;
}

atUint32 Sprite::currentState() const
{
    return m_currentState;
}

bool Sprite::addFrame(SpriteFrame* part)
{
    if (m_frames.size() > 65536)
        return false;

    for (SpriteFrame* tmp : m_frames)
    {
        if (tmp == part)
            return false;
    }

    m_frames.push_back(part);
    return true;
}

bool Sprite::removeFrame(SpriteFrame* frame)
{
    std::vector<SpriteFrame*>::iterator iter = std::find(m_frames.begin(), m_frames.end(), frame);

    if (iter != m_frames.end())
    {
        m_frames.erase(iter);
        return true;
    }
    return false;
}

void Sprite::setFrame(atUint32 id)
{
    if (id > m_frames.size())
        return;
}

void Sprite::setFrames(std::vector<SpriteFrame*> frames)
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

    m_frames = frames;
}


atUint32 Sprite::frameCount() const
{
    return (atUint32)m_frames.size();
}

std::vector<SpriteFrame*> Sprite::frames() const
{
    return m_frames;
}

SpriteFile* Sprite::container() const
{
    return m_root;
}

void Sprite::setCurrentFrame(SpriteFrame* frame)
{
    atUint32 id = 0;

    for (SpriteFrame* tmpFrame : m_frames)
    {
        if (tmpFrame == frame)
        {
            setCurrentFrame(id);
            return;
        }

        id++;
    }
}

void Sprite::setCurrentFrame(atUint32 id)
{
    if (id >= m_frames.size())
        return;

    m_currentFrame = id;
}

SpriteFrame* Sprite::currentFrame() const
{
    return m_frames[m_currentFrame];
}

void Sprite::advanceFrame()
{
    m_currentFrame++;

    if (m_currentFrame >= m_frames.size())
        m_currentFrame = (atUint32)m_frames.size() - 1;
}

void Sprite::retreatFrame()
{
    if (m_currentFrame == 0)
        return;

    m_currentFrame--;
}

void Sprite::setRoot(SpriteFile* root)
{
    m_root = root;
}

SpriteFile* Sprite::root() const
{
    return m_root;
}
}
