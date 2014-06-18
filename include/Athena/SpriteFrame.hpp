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

#ifndef SSPRITEFRAME_HPP
#define SSPRITEFRAME_HPP


#include "Athena/Global.hpp"

#ifndef ATHENA_USE_QT
#include <vector>
#else
#include <QObject>
#include <QList>
#endif

namespace Athena
{
namespace Sakura
{

class Sprite;
class SpritePart;

#ifndef ATHENA_USE_QT
class SpriteFrame
{
#else
class SpriteFrame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal frameTime READ frameTime WRITE setFrameTime)
#endif
public:
    /*!
     * \brief SSpriteFrame
     */
    SpriteFrame();

    SpriteFrame(Sprite* root);
    /*!
     * \brief setFrameTime
     * \param frameTime
     */
    void setFrameTime(float frameTime);


    /*!
     * \brief frameTime
     * \return
     */
    float frameTime() const;

#ifndef ATHENA_USE_QT
    void setParts(std::vector<SpritePart*> parts);
    std::vector<SpritePart*> parts() const;
#else
    void setParts(QList<SpritePart*> parts);
    QList<SpritePart*> parts() const;
#endif

    atUint32 partCount() const;

    void setRoot(Sprite* root);
    Sprite* root() const;
#ifdef ATHENA_USE_QT
signals:
    void frameTimeChanged(float);
#endif
private:
    Sprite*                  m_root;
    float                    m_frameTime;
#ifndef ATHENA_USE_QT
    std::vector<SpritePart*> m_parts;
#else
    QList<SpritePart*>       m_parts;
#endif
};

} // Sakura
} // zelda

#ifdef ATHENA_USE_QT
Q_DECLARE_METATYPE(Athena::Sakura::SpriteFrame*);
#endif

#endif // SSPRITEFRAME_HPP
