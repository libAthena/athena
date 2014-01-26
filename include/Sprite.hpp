#ifndef SSPRITE_HPP
#define SSPRITE_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <Types.hpp>

namespace zelda
{
namespace Sakura
{
class SpriteFile;
class SpritePart;

class Sprite
{
public:
    Sprite(SpriteFile* root);
    Sprite(SpriteFile* root, const std::string& name);
    virtual ~Sprite();

    virtual void setPosition(const float x, const float y);
    virtual void setPosition(const Vector2Df& pos);
    Vector2Df position() const;
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
    Uint32 stateCount() const;
    void setCurrentState(const Uint32 id);

    void addPart(SpritePart* part);
    SpritePart* part(const std::string& name);
    void setParts(std::vector<SpritePart*> parts);
    Uint32 partCount() const;

    std::vector<SpritePart*> parts() const;

    SpriteFile* container() const;
private:
    SpriteFile* m_root;
    std::string m_name;
    Vector2Df   m_position;

    std::vector<int> m_stateIds; //!< Stores the texture id's for each state.
    std::vector<SpritePart*> m_parts;
    Uint32 m_currentState;
};


} // Sakura
} // zelda

#endif // SSPRITE_HPP
