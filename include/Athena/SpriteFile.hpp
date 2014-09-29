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

#ifndef SSPRITEFILE_HPP
#define SSPRITEFILE_HPP

#ifndef ATHENA_USE_QT
#include <vector>
#include <unordered_map>
#else
#include <QObject>
#include <QMap>
#include <QList>
#include <QSize>
#include <QPoint>
#endif

#include <string>
#include "Athena/Global.hpp"

namespace Athena
{
namespace Sakura
{
struct STexture
{
    std::string Filepath;
    bool        Preload;
};

class Sprite;
#ifndef ATHENA_USE_QT
class SpriteFile
{
#else
class SpriteFile : public QObject
{
    Q_OBJECT
#endif
public:
    /*!
     * \brief Major
     */
    static const atUint32 Major;

    /*!
     * \brief Minor
     */
    static const atUint32 Minor;

    /*!
     * \brief Revision
     */
    static const atUint32 Revision;

    /*!
     * \brief Patch
     */
    static const atUint32 Build;

    /*!
     * \brief Version
     */
    static const atUint32 Version;

    /*!
     * \brief Magic
     */
    static const atUint32 Magic;

    /*!
     * \brief SSprite
     */
    SpriteFile();

    /*!
     * \brief SSpriteFile
     * \param width
     * \param height
     * \param originX
     * \param originY
     */
    SpriteFile(atUint32 width, atUint32 height, float originX, float originY);

    /*!
     * \brief SSpriteFile
     * \param size
     * \param origin
     */
#ifndef ATHENA_USE_QT
    SpriteFile(const Vector2Di& size, const Vector2Df& origin);
#else
    SpriteFile(const QSize& size, const QPoint& origin);
#endif

    ~SpriteFile();

#ifndef ATHENA_USE_QT
public:
#else
public slots:
#endif

    /*!
     * \brief setSize
     * \param width
     * \param height
     */
    void setSize(atUint32 width, atUint32 height);

    /*!
     * \brief setSize
     * \param size
     */
#ifndef ATHENA_USE_QT
    void setSize(const Vector2Di& size);
#else
    void setSize(const QSize& size);
#endif

    /*!
     * \brief size
     * \return
     */
#ifndef ATHENA_USE_QT
    Vector2Di size() const;
#else
    QSize size() const;
#endif

    /*!
     * \brief width
     * \return
     */
    atUint32 width() const;

    /*!
     * \brief height
     * \return
     */
    atUint32 height() const;

    /*!
     * \brief setOrigin
     * \param x
     * \param y
     */
    void setOrigin(const float x, const float y);

    /*!
     * \brief setOrigin
     * \param origin
     */
#ifndef ATHENA_USE_QT
    void setOrigin(const Vector2Df& origin);
#else
    void setOrigin(const QPoint& origin);
#endif

    /*!
     * \brief origin
     * \return
     */
#ifndef ATHENA_USE_QT
    Vector2Df origin() const;
#else
    QPoint origin() const;
#endif


    /*!
     * \brief originX
     * \return
     */
    float originX() const;

    /*!
     * \brief originY
     * \return
     */
    float originY() const;

    /*!
     * \brief addTexture
     * \param texture
     */
    bool addTexture(STexture* texture);

    /*!
     * \brief removeTexture
     * \param id
     */
    void removeTexture(int id);

    /*!
     * \brief texture
     * \param id
     * \return
     */
    STexture* texture(atUint32 id);

#ifndef ATHENA_USE_QT
    std::vector<STexture*> textures() const;
#else
    QList<STexture*> textures() const;
#endif
    atUint32 textureCount() const;
    /*!
     * \brief setTextures
     * \param textures
     */

#ifndef ATHENA_USE_QT
    void setTextures(std::vector<STexture*> textures);
#else
    void setTextures(QList<STexture*> textures);
#endif

    void addSprite(Sprite* sprite);

#ifndef ATHENA_USE_QT
    void removeSprite(const std::string& name);
#else
    void removeSprite(const QString& name);
#endif
    void removeSprite(Sprite* sprite);

#ifndef ATHENA_USE_QT
    void setSprites(std::unordered_map<std::string, Sprite*> sprites);
#else
    void setSprites(QMap<QString, Sprite*> sprites);
#endif
#ifndef ATHENA_USE_QT
    Sprite* sprite(const std::string& name);
    std::unordered_map<std::string, Sprite*> sprites() const;
#else
    Sprite* sprite(const QString& name);
    QMap<QString, Sprite*> sprites() const;
#endif

    atUint32 spriteCount() const;

#ifdef ATHENA_USE_QT
signals:
    void originChanged(QPoint);
    void sizeChanged(QSize);
#endif
    private:
    #ifndef ATHENA_USE_QT
        std::vector<STexture*> m_textures;
    Vector2Di              m_size;
    Vector2Df              m_origin;
    std::unordered_map<std::string, Sprite*> m_sprites;
#else
    QList<STexture*>       m_textures;
    QSize                  m_size;
    QPoint                 m_origin;
    QMap<QString, Sprite*> m_sprites;
#endif
};
} // Sakura
} // Zelda

#ifdef ATHENA_USE_QT
Q_DECLARE_METATYPE(Athena::Sakura::SpriteFile*)
Q_DECLARE_METATYPE(Athena::Sakura::STexture*)
#endif
#endif // SSPRITE_HPP

#endif // ATHENA_NO_SAKURA

