#ifndef WIIIMAGE_HPP
#define WIIIMAGE_HPP

#include <memory>
#include "Athena/Types.hpp"

namespace Athena
{

/*!
 * \brief The WiiImage class
 */
class WiiImage
{
public:
    /*!
     * \brief WiiImage
     */
    WiiImage();
    /*!
     * \brief WiiImage
     * \param width
     * \param height
     * \param data
     */
    WiiImage(atUint32 width, atUint32 height, std::unique_ptr<atUint8[]>&& data);

    /*!
     * \brief setWidth
     * \param width
     */
    void setWidth(const atUint32 width);

    /*!
     * \brief width
     * \return
     */
    atUint32  width() const;

    /*!
     * \brief setHeight
     * \param height
     */
    void  setHeight(const atUint32 height);

    /*!
     * \brief height
     * \return
     */
    atUint32   height() const;

    /*!
     * \brief setData
     * \param data
     */
    void  setData(const atUint8* data);

    /*!
     * \brief data
     * \return
     */
    atUint8*   data();

    /*!
     * \brief toRGBA
     * \return
     */
    atUint8* toRGBA();

private:
    atUint32 m_width;
    atUint32 m_height;
    std::unique_ptr<atUint8[]> m_data;
};

} // zelda

#endif // WIIIMAGE_HPP
