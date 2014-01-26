#ifndef SSPRITEFRAME_HPP
#define SSPRITEFRAME_HPP


#include <Types.hpp>

#include <vector>

namespace zelda
{
namespace Sakura
{

class SpriteFrame
{
public:
    /*!
     * \brief SSpriteFrame
     */
    SpriteFrame();

    /*!
     * \brief SSpriteFrame
     * \param offX
     * \param offY
     * \param texX
     * \param texY
     * \param width
     * \param height
     * \param frameTime
     * \param flippedH
     * \param flippedV
     */
    SpriteFrame(float offX, float offY, float texX, float texY, Uint32 width, Uint32 height, float frameTime, bool flippedH = false, bool flippedV = false);

    /*!
     * \brief SSpriteFrame
     * \param frameOff
     * \param texOff
     * \param size
     * \param frameTime
     * \param flippedH
     * \param flippedV
     */
    SpriteFrame(const Vector2Df& frameOff, const Vector2Df& texOff, const Vector2Di& size, float frameTime, bool flippedH = false, bool flippedV = false);

    /*!
     * \brief setOffset
     * \param x
     * \param y
     */
    void setOffset(float x, float y);

    /*!
     * \brief setOffset
     * \param offset
     */
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
    void setTextureOffset(const Vector2Df& texOff);

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
    void setSize(Uint32 width, Uint32 height);

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

private:
    Vector2Df m_offset;
    Vector2Df m_textureOffset;
    Vector2Di m_size;
    float     m_frameTime;
    bool      m_flippedH;
    bool      m_flippedV;
};

} // Sakura
} // zelda

#endif // SSpRITEFRAME_HPP
