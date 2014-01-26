#ifndef SSPRITEPART_HPP
#define SSPRITEPART_HPP

#include <Types.hpp>
#include <vector>
#include <string>
#include <Types.hpp>

namespace zelda
{
namespace Sakura
{
class Sprite;
class SpriteFrame;

class SpritePart
{
public:
    SpritePart(Sprite* root);
    SpritePart(Sprite* root, const std::string& name, bool hasCollision = false);
    virtual ~SpritePart();

    void setName(const std::string& name);
    std::string name() const;

    void setCollision(bool col);
    bool hasCollision() const;

    void addFrame(SpriteFrame* frame);
    void advanceFrame();
    void retreatFrame();
    SpriteFrame* frame(int id);
    void setFrames(std::vector<SpriteFrame*> frames);
    SpriteFrame* currentFrame();
    int currentFrameID();
    std::vector<SpriteFrame*> frames() const;
    Uint32 frameCount() const;


    void setRoot(Sprite* root);
private:
    Sprite*      m_root;
    std::string   m_name;
    bool          m_hasCollision;
    SpriteFrame*  m_currentFrame;
    Uint32        m_frameIndex;

    // The collection of frames for this part
    std::vector<SpriteFrame*> m_frames;
};

}
}

#endif // SSpRITEPART_HPP
