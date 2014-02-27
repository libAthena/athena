#include "Sprite.hpp"
#include "SpriteFrame.hpp"
#include "SpriteFile.hpp"
#include <algorithm>

#ifdef LIBZELDA_USE_QT
#include <QVector>
#endif

namespace zelda
{
namespace Sakura
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
#ifdef LIBZELDA_USE_QT
    m_name = QString::fromStdString(name);
#else
    m_name = name;
#endif
}

Sprite::~Sprite()
{
#ifndef LIBZELDA_USE_QT
    for (SpriteFrame* frame : m_frames)
#else
    foreach(SpriteFrame* frame, m_frames)
#endif
    {
        delete frame;
        frame = NULL;
    }

    m_frames.clear();
}

void Sprite::setPosition(const float x, const float y)
{
#ifndef LIBZELDA_USE_QT
    setPosition(Vector2Df(x, y));
#else
    setPosition(QPoint(x, y));
#endif
}

#ifndef LIBZELDA_USE_QT
void Sprite::setPosition(const Vector2Df& pos)
#else
void Sprite::setPosition(const QPoint& pos)
#endif
{
    m_position = pos;
}

#ifndef LIBZELDA_USE_QT
Vector2Df Sprite::position() const
#else
QPoint Sprite::position() const
#endif
{
    return m_position;
}

#ifndef LIBZELDA_USE_QT
void Sprite::setName(const std::string& name)
#else
void Sprite::setName(const QString& name)
#endif
{
    m_name = name;

#ifdef LIBZELDA_USE_QT
    emit nameChanged(name);
#endif
}

#ifndef LIBZELDA_USE_QT
std::string Sprite::name() const
#else
QString Sprite::name() const
#endif
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

#ifndef LIBZELDA_USE_QT
    m_stateIds = ids;
#else
    m_stateIds = QList<int>::fromVector(QVector<int>::fromStdVector(ids));
#endif
}

#ifndef LIBZELDA_USE_QT
std::vector<int> Sprite::stateIds() const
#else
QList<int> Sprite::stateIds() const
#endif
{
    return m_stateIds;
}

Uint32 Sprite::stateCount() const
{
    return m_stateIds.size();
}

void Sprite::setCurrentState(const Uint32 id)
{
    if (id >= m_stateIds.size())
        return;

    m_currentState = id;
#ifdef LIBZELDA_USE_QT
    emit stateChanged(id);
#endif
}

Uint32 Sprite::currentState() const
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
#ifndef LIBZELDA_USE_QT
    std::vector<SpriteFrame*>::iterator iter = std::find(m_frames.begin(), m_frames.end(), frame);
    if (iter != m_frames.end())
    {
        m_frames.erase(iter);
        return true;
    }
#else
    if (m_frames.removeOne(frame))
        return true;
#endif
    return false;
}

void Sprite::setFrame(Uint32 id)
{
    if (id > m_frames.size())
        return;
}

#ifndef LIBZELDA_USE_QT
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
#else
void Sprite::setFrames(QList<SpriteFrame*> frames)
{
    m_frames.clear();
    m_frames = frames;
}
#endif


Uint32 Sprite::frameCount() const
{
    return m_frames.size();
}

#ifndef LIBZELDA_USE_QT
std::vector<SpritePart*> Sprite::frames() const
#else
QList<SpriteFrame*> Sprite::frames() const
#endif
{
    return m_frames;
}

SpriteFile* Sprite::container() const
{
    return m_root;
}

void Sprite::setCurrentFrame(SpriteFrame* frame)
{
    Uint32 id = 0;
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

void Sprite::setCurrentFrame(Uint32 id)
{
    if (id >= m_frames.size())
        return;

    m_currentFrame = id;
#ifdef LIBZELDA_USE_QT
    emit frameChanged(currentFrame());
#endif
}

SpriteFrame* Sprite::currentFrame() const
{
    return m_frames[m_currentFrame];
}

void Sprite::advanceFrame()
{
    m_currentFrame++;
    if (m_currentFrame >= m_frames.size())
        m_currentFrame = m_frames.size() - 1;
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
}
