#pragma once

#include <vector>
#include <string>
#include "athena/SakuraGlobal.hpp"

namespace athena::Sakura {
class SpriteFile;
class SpriteFrame;

class Sprite {
public:
  Sprite(SpriteFile* root);
  Sprite(SpriteFile* root, const std::string& name);
  virtual ~Sprite();

  virtual void setPosition(const float x, const float y);
  virtual void setPosition(const Vector2Df& pos);
  virtual Vector2Df position() const;
  void setName(const std::string& name);
  std::string name() const;

  void addStateId(int id);

  /*!
   * \brief Returns the texture id of a given state
   * \param index The index of the id.
   * \return return the state id if it exists, -1 otherwise
   */
  int stateId(int index) const;
  void setStateIds(std::vector<int> ids);
  std::vector<int> stateIds() const;
  atUint32 stateCount() const;
  void setCurrentState(atUint32 id);
  atUint32 currentState() const;

  bool addFrame(SpriteFrame* Frame);
  bool removeFrame(SpriteFrame* Frame);
  SpriteFrame* Frame(atUint32 id);
  void setFrame(atUint32 id);
  void setFrames(std::vector<SpriteFrame*> frames);
  atUint32 frameCount() const;

  std::vector<SpriteFrame*> frames() const;

  SpriteFile* container() const;

  void setCurrentFrame(SpriteFrame* frame);
  void setCurrentFrame(atUint32 id);
  SpriteFrame* currentFrame() const;

  void advanceFrame();
  void retreatFrame();

  void setRoot(SpriteFile* root);
  SpriteFile* root() const;

private:
  SpriteFile* m_root;
  std::string m_name;
  Vector2Df m_position;
  std::vector<int> m_stateIds; //!< Stores the texture id's for each state.
  std::vector<SpriteFrame*> m_frames;
  atUint32 m_currentState;
  atUint32 m_currentFrame;
};

} // namespace athena::Sakura
