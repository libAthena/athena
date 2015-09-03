#ifndef SSPRITEFRAME_HPP
#define SSPRITEFRAME_HPP


#include "Athena/SakuraGlobal.hpp"

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
