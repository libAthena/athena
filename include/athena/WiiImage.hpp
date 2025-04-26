#pragma once

#include <memory>
#include "athena/Types.hpp"

namespace athena {

/*!
 * \brief The WiiImage class
 */
class WiiImage {
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
  WiiImage(uint32_t width, uint32_t height, std::unique_ptr<uint8_t[]>&& data);

  /*!
   * \brief setWidth
   * \param width
   */
  void setWidth(const uint32_t width);

  /*!
   * \brief width
   * \return
   */
  uint32_t width() const;

  /*!
   * \brief setHeight
   * \param height
   */
  void setHeight(const uint32_t height);

  /*!
   * \brief height
   * \return
   */
  uint32_t height() const;

  /*!
   * \brief setData
   * \param data
   */
  void setData(const uint8_t* data);

  /*!
   * \brief data
   * \return
   */
  uint8_t* data();

  /*!
   * \brief toRGBA
   * \return
   */
  uint8_t* toRGBA();

private:
  uint32_t m_width;
  uint32_t m_height;
  std::unique_ptr<uint8_t[]> m_data;
};

} // namespace athena
