#include "Sprite.hpp"
#include "SpritePart.hpp"
#include "SpriteFile.hpp"
#include <algorithm>

namespace zelda
{
namespace Sakura
{
Sprite::Sprite(SpriteFile* root)
    : m_root(root),
      m_currentState(0)
{
}

Sprite::Sprite(SpriteFile* root, const std::string& name)
    : m_root(root),
      m_name(name),
      m_currentState(0)
{
}

Sprite::~Sprite()
{

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

Uint32 Sprite::stateCount() const
{
    return m_stateIds.size();
}

void Sprite::setCurrentState(const Uint32 id)
{
    m_currentState = id;
}

void Sprite::addPart(SpritePart* part)
{
    for (SpritePart* tmp : m_parts)
    {
        if (tmp == part)
            return;
    }
    m_parts.push_back(part);
}

void Sprite::setParts(std::vector<SpritePart*> parts)
{
    if (parts.size() == 0)
        return;

    if (m_parts.size() > 0)
    {
        for (SpritePart* part : m_parts)
        {
            delete part;
            part = NULL;
        }
        m_parts.clear();
    }

    m_parts = parts;
}

Uint32 Sprite::partCount() const
{
    return m_parts.size();
}

std::vector<SpritePart*> Sprite::parts() const
{
    return m_parts;
}

SpriteFile* Sprite::container() const
{
    return m_root;
}
}
}
