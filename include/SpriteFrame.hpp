#ifndef SSPRITEFRAME_HPP
#define SSPRITEFRAME_HPP


#include <Types.hpp>

#ifndef LIBZELDA_USE_QT
#include <vector>
#else
#include <QObject>
#include <QList>
#endif

namespace zelda
{
namespace Sakura
{

class Sprite;
class SpritePart;

#ifndef LIBZELDA_USE_QT
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

#ifndef LIBZELDA_USE_QT
    void setParts(std::vector<SpritePart*> parts);
    std::vector<SpritePart*> parts() const;
#else
    void setParts(QList<SpritePart*> parts);
    QList<SpritePart*> parts() const;
#endif

    Uint32 partCount() const;

    void setRoot(Sprite* root);
    Sprite* root() const;
#ifdef LIBZELDA_USE_QT
signals:
    void frameTimeChanged(float);
#endif
private:
    Sprite*                  m_root;
    float                    m_frameTime;
#ifndef LIBZELDA_USE_QT
    std::vector<SpritePart*> m_parts;
#else
    QList<SpritePart*>       m_parts;
#endif
};

} // Sakura
} // zelda

#ifdef LIBZELDA_USE_QT
Q_DECLARE_METATYPE(zelda::Sakura::SpriteFrame*);
#endif

#endif // SSPRITEFRAME_HPP
