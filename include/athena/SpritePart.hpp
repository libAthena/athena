#pragma once

#include "athena/SakuraGlobal.hpp"
#include <vector>

#include <string>

namespace athena::Sakura {
class SpriteFrame;

class SpritePart {
public:
  SpritePart(SpriteFrame* root);
  SpritePart(SpriteFrame* root, const std::string& name, bool hasCollision = false);
  virtual ~SpritePart();

  void setName(const std::string& name);
  std::string name() const;
  void setCollision(bool col);
  bool hasCollision() const;

  /*!
   * \brief setOffset
   * \param x
   * \param y
   */
  void setOffset(float x, float y);
  void setOffset(const Vector2Df& offset);

  /*!
   * \brief offset
   * \return
   */
  Vector2Df offset() const;

  /*!
   * \brief setTextureOffset
   * \param x
   * \param y
   */
  void setTextureOffset(float x, float y);

  /*!
   * \brief setTextureOffset
   * \param texOff
   */
  void setTextureOffset(const Vector2Df& offset);

  /*!
   * \brief textureOffset
   * \return
   */
  Vector2Df textureOffset() const;
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
   * \brief setFlippedHorizontally
   * \param val
   */
  void setFlippedHorizontally(const bool val);

  /*!
   * \brief flippedHorizontally
   * \return
   */
  bool flippedHorizontally() const;

  /*!
   * \brief setFlippedVertically
   * \param val
   */
  void setFlippedVertically(const bool val);

  /*!
   * \brief flippedVertically
   * \return
   */
  bool flippedVertically() const;

  void setRoot(SpriteFrame* root);
  SpriteFrame* root() const;

private:
  SpriteFrame* m_root;
  std::string m_name;
  bool m_hasCollision;
  Vector2Df m_offset;
  Vector2Df m_textureOffset;
  Vector2Di m_size;
  bool m_flippedH;
  bool m_flippedV;
  uint32_t m_frameIndex;
};
} // namespace athena::Sakura
