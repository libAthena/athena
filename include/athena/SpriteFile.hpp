#ifndef SSPRITEFILE_HPP
#define SSPRITEFILE_HPP

#include <vector>
#include <unordered_map>

#include <string>
#include "athena/SakuraGlobal.hpp"

namespace athena::Sakura {
struct STexture {
  std::string Filepath;
  bool Preload;
};

class Sprite;
class SpriteFile {
public:
  /*!
   * \brief Major
   */
  static const uint32_t Major;

  /*!
   * \brief Minor
   */
  static const uint32_t Minor;

  /*!
   * \brief Revision
   */
  static const uint32_t Revision;

  /*!
   * \brief Patch
   */
  static const uint32_t Build;

  /*!
   * \brief Version
   */
  static const uint32_t Version;

  /*!
   * \brief Magic
   */
  static const uint32_t Magic;

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
  SpriteFile(uint32_t width, uint32_t height, float originX, float originY);

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
  void setSize(uint32_t width, uint32_t height);

  /*!
   * \brief setSize
   * \param size
   */
  void setSize(const Vector2Di& size);

  /*!
   * \brief size
   * \return
   */
  Vector2Di size() const;

  /*!
   * \brief width
   * \return
   */
  uint32_t width() const;

  /*!
   * \brief height
   * \return
   */
  uint32_t height() const;

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
  void setOrigin(const Vector2Df& origin);

  /*!
   * \brief origin
   * \return
   */
  Vector2Df origin() const;

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
  STexture* texture(uint32_t id);
  std::vector<STexture*> textures() const;

  uint32_t textureCount() const;
  /*!
   * \brief setTextures
   * \param textures
   */
  void setTextures(std::vector<STexture*> textures);

  void addSprite(Sprite* sprite);

  void removeSprite(const std::string& name);
  void removeSprite(Sprite* sprite);

  void setSprites(std::unordered_map<std::string, Sprite*> sprites);
  Sprite* sprite(const std::string& name);
  std::unordered_map<std::string, Sprite*> sprites() const;

  uint32_t spriteCount() const;

private:
  std::vector<STexture*> m_textures;
  Vector2Di m_size;
  Vector2Df m_origin;
  std::unordered_map<std::string, Sprite*> m_sprites;
};
} // namespace athena::Sakura

#endif // SSPRITE_HPP
