#ifndef SSPRITE_HPP
#define SSPRITE_HPP

#ifndef  LIBZELDA_USE_QT
#include <vector>
#include <string>
#else
#include <QObject>
#include <QPoint>
#include <QString>
#include <QList>
#endif
#include <Types.hpp>

namespace zelda
{
namespace Sakura
{
class SpriteFile;
class SpriteFrame;

#ifndef LIBZELDA_USE_QT
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
#ifndef LIBZELDA_USE_QT
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
#ifndef LIBZELDA_USE_QT
    std::vector<int> stateIds() const;
#else
    QList<int> stateIds() const;
#endif
    Uint32 stateCount() const;
    void setCurrentState(Uint32 id);
    Uint32 currentState() const;

    bool addFrame(SpriteFrame* Frame);
    bool removeFrame(SpriteFrame* Frame);
    SpriteFrame* Frame(Uint32 id);
    void setFrame(Uint32 id);
#ifndef LIBZELDA_USE_QT
    void setFrames(std::vector<SpriteFrame*> frames);
#else
    void setFrames(QList<SpriteFrame*> frames);
#endif
    Uint32 frameCount() const;

#ifndef LIBZELDA_USE_QT
    std::vector<SpriteFrame*> frames() const;
#else
    QList<SpriteFrame*> frames() const;
#endif

    SpriteFile* container() const;

    void setCurrentFrame(SpriteFrame* frame);
    void setCurrentFrame(Uint32 id);
    SpriteFrame* currentFrame() const;

    void advanceFrame();
    void retreatFrame();

    void setRoot(SpriteFile* root);
    SpriteFile* root() const;
#ifdef LIBZELDA_USE_QT
signals:
    void frameChanged(SpriteFrame* frame);
    void nameChanged(QString);
    void stateChanged(quint32);
#endif
private:
    SpriteFile*               m_root;
#ifndef LIBZELDA_USE_QT
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
    Uint32                    m_currentState;
    Uint32                    m_currentFrame;
};


} // Sakura
} // zelda

#ifdef LIBZELDA_USE_QT
Q_DECLARE_METATYPE(Uint32)
Q_DECLARE_METATYPE(zelda::Sakura::Sprite*)
#endif

#endif // SSPRITE_HPP
