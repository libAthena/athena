#ifndef ATHENA_NO_SAKURA
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

#include "Athena/SpriteFrame.hpp"
#include "Athena/SpritePart.hpp"
#include "Athena/Sprite.hpp"

namespace Athena
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
#endif // ATHENA_NO_SAKURA
