#include "athena/SpriteFrame.hpp"
#include "athena/SpritePart.hpp"
#include "athena/Sprite.hpp"

namespace athena::Sakura {

SpriteFrame::SpriteFrame() {}

SpriteFrame::SpriteFrame(Sprite* root) : m_root(root) {}

void SpriteFrame::setFrameTime(float frameTime) { m_frameTime = frameTime; }

void SpriteFrame::setParts(std::vector<SpritePart*> parts) { m_parts = parts; }

std::vector<SpritePart*> SpriteFrame::parts() const { return m_parts; }

uint32_t SpriteFrame::partCount() const { return (uint32_t)m_parts.size(); }

float SpriteFrame::frameTime() const { return m_frameTime; }

void SpriteFrame::setRoot(Sprite* root) {
  root->removeFrame(this);
  m_root = root;
}

Sprite* SpriteFrame::root() const { return m_root; }

} // namespace athena::Sakura
