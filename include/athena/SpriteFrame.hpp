#pragma once

#include "athena/SakuraGlobal.hpp"

#include <vector>
namespace athena::Sakura {

class Sprite;
class SpritePart;

class SpriteFrame {
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

  void setParts(std::vector<SpritePart*> parts);
  std::vector<SpritePart*> parts() const;

  uint32_t partCount() const;

  void setRoot(Sprite* root);
  Sprite* root() const;

private:
  Sprite* m_root;
  float m_frameTime;
  std::vector<SpritePart*> m_parts;
};

} // namespace athena::Sakura
