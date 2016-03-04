#ifndef SSPRITE_HPP
#define SSPRITE_HPP

#ifndef  ATHENA_USE_QT
#include <vector>
#include <string>
#else
#include <QObject>
#include <QPoint>
#include <QString>
#include <QList>
#endif
#include "athena/SakuraGlobal.hpp"

namespace athena
{
namespace Sakura
{
class SpriteFile;
class SpriteFrame;

#ifndef ATHENA_USE_QT
class Sprite
{
#else
class Sprite : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(qreal currentState READ currentState WRITE setCurrentState)
    Q_PROPERTY(qreal stateCount READ stateCount CONSTANT)
#endif

public:
    Sprite(SpriteFile* root);
    Sprite(SpriteFile* root, const std::string& name);
    virtual ~Sprite();

    virtual void setPosition(const float x, const float y);
#ifndef ATHENA_USE_QT
    virtual void setPosition(const Vector2Df& pos);
    virtual Vector2Df position() const;
    void setName(const std::string& name);
    std::string name() const;
#else
    virtual void setPosition(const QPoint& pos);
    virtual QPoint position() const;
    void setName(const QString& name);
    QString name() const;
#endif

    void addStateId(int id);

    /*!
     * \brief Returns the texture id of a given state
     * \param index The index of the id.
     * \return return the state id if it exists, -1 otherwise
     */
    int stateId(int index) const;
    void setStateIds(std::vector<int> ids);
#ifndef ATHENA_USE_QT
    std::vector<int> stateIds() const;
#else
    QList<int> stateIds() const;
#endif
    atUint32 stateCount() const;
    void setCurrentState(atUint32 id);
    atUint32 currentState() const;

    bool addFrame(SpriteFrame* Frame);
    bool removeFrame(SpriteFrame* Frame);
    SpriteFrame* Frame(atUint32 id);
    void setFrame(atUint32 id);
#ifndef ATHENA_USE_QT
    void setFrames(std::vector<SpriteFrame*> frames);
#else
    void setFrames(QList<SpriteFrame*> frames);
#endif
    atUint32 frameCount() const;

#ifndef ATHENA_USE_QT
    std::vector<SpriteFrame*> frames() const;
#else
    QList<SpriteFrame*> frames() const;
#endif

    SpriteFile* container() const;

    void setCurrentFrame(SpriteFrame* frame);
    void setCurrentFrame(atUint32 id);
    SpriteFrame* currentFrame() const;

    void advanceFrame();
    void retreatFrame();

    void setRoot(SpriteFile* root);
    SpriteFile* root() const;
#ifdef ATHENA_USE_QT
signals:
    void frameChanged(SpriteFrame* frame);
    void nameChanged(QString);
    void stateChanged(quint32);
#endif
private:
    SpriteFile*               m_root;
#ifndef ATHENA_USE_QT
    std::string               m_name;
    Vector2Df                 m_position;
    std::vector<int>          m_stateIds; //!< Stores the texture id's for each state.
    std::vector<SpriteFrame*> m_frames;
#else
    QString                   m_name;
    QPoint                    m_position;
    QList<int>                m_stateIds;
    QList<SpriteFrame*>       m_frames;
#endif
    atUint32                    m_currentState;
    atUint32                    m_currentFrame;
};


} // Sakura
} // zelda

#ifdef ATHENA_USE_QT
Q_DECLARE_METATYPE(Uint32)
Q_DECLARE_METATYPE(athena::Sakura::Sprite*)
#endif

#endif // SSPRITE_HPP
