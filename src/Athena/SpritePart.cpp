// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

#include "Athena/SpritePart.hpp"
#include "Athena/SpritePart.hpp"
#include "Athena/Sprite.hpp"
#include <algorithm>

namespace Athena
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
#ifdef ATHENA_USE_QT
    m_name = QString::fromStdString(name);
#else
    m_name = name;
#endif
}

SpritePart::~SpritePart()
{
}

#ifndef ATHENA_USE_QT
void SpritePart::setName(const std::string& name)
#else
void SpritePart::setName(const QString& name)
#endif
{
    m_name = name;
#ifdef ATHENA_USE_QT
    emit nameChanged(name);
#endif
}

#ifndef ATHENA_USE_QT
std::string SpritePart::name() const
#else
QString SpritePart::name() const
#endif
{
    return m_name;
}

void SpritePart::setCollision(bool col)
{
    m_hasCollision = col;
#ifdef ATHENA_USE_QT
    emit collisionChanged(col);
#endif
}

bool SpritePart::hasCollision() const
{
    return m_hasCollision;
}

void SpritePart::setOffset(float x, float y)
{
#ifndef ATHENA_USE_QT
    setOffset(Vector2Df(x, y));
#else
    setOffset(QPoint(x, y));
#endif
}

#ifndef ATHENA_USE_QT
void SpritePart::setOffset(const Vector2Df& offset)
#else
void SpritePart::setOffset(const QPoint& offset)
#endif
{
    m_offset = offset;
#ifdef ATHENA_USE_QT
    emit offsetChanged(offset);
#endif
}

#ifndef ATHENA_USE_QT
Vector2Df SpritePart::offset() const
#else
QPoint SpritePart::offset() const
#endif
{
    return m_offset;
}

void SpritePart::setTextureOffset(float x, float y)
{
#ifndef ATHENA_USE_QT
    setTextureOffset(Vector2Df(x, y));
#else
    setTextureOffset(QPoint(x, y));
#endif
}

#ifndef ATHENA_USE_QT
void SpritePart::setTextureOffset(const Vector2Df& offset)
#else
void SpritePart::setTextureOffset(const QPoint& offset)
#endif
{
    m_textureOffset = offset;
#ifdef ATHENA_USE_QT
    emit textureOffsetChanged(offset);
#endif
}

#ifndef ATHENA_USE_QT
Vector2Df SpritePart::textureOffset() const
#else
QPoint SpritePart::textureOffset() const
#endif
{
    return m_textureOffset;
}

void SpritePart::setSize(Uint32 width, Uint32 height)
{
#ifndef ATHENA_USE_QT
    setSize(Vector2Di(width, height));
#else
    setSize(QSize(width, height));
#endif
}

#ifndef ATHENA_USE_QT
void SpritePart::setSize(const Vector2Di& size)
#else
void SpritePart::setSize(const QSize& size)
#endif
{
    m_size = size;
#ifdef ATHENA_USE_QT
    emit sizeChanged(size);
#endif
}

#ifndef ATHENA_USE_QT
Vector2Di SpritePart::size() const
#else
QSize SpritePart::size() const
#endif
{
    return m_size;
}

void SpritePart::setFlippedHorizontally(const bool val)
{
    m_flippedH = val;
#ifdef ATHENA_USE_QT
    emit orientationChanged(val, flippedVertically());
#endif
}

bool SpritePart::flippedHorizontally() const
{
    return m_flippedH;
}

void SpritePart::setFlippedVertically(const bool val)
{
    m_flippedV = val;
#ifdef ATHENA_USE_QT
    emit orientationChanged(flippedHorizontally(), val);
#endif
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
