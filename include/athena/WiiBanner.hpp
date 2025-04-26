#pragma once

#include <vector>
#include <string>
#include "athena/Global.hpp"

namespace athena {
class WiiImage;
/*! \class WiiBanner
 *  \brief Wii banner container class
 *
 * Contains all relevant data for a Wii banner.
 */
class WiiBanner {
public:
  enum { NoCopy = 0x00000001, Bounce = 0x00000010, NoCopyBounce = NoCopy | Bounce };

  /*!
   * \brief WiiBanner
   */
  WiiBanner();
  /*!
   * \brief WiiBanner
   * \param gameId
   * \param title
   * \param subtitle
   * \param m_banner
   * \param icons
   */
  WiiBanner(uint32_t gameId, const std::u16string& title, const std::u16string& subtitle, WiiImage* m_banner,
            std::vector<WiiImage*> icons);
  virtual ~WiiBanner();

  /*!
   * \brief setGameID
   * \param id
   */
  void setGameID(uint64_t id);

  /*!
   * \brief gameID
   * \return
   */
  uint64_t gameID() const;

  /*!
   * \brief setBannerImage
   * \param banner
   */
  void setBannerImage(WiiImage* banner);

  /*!
   * \brief bannerImage
   * \return
   */
  WiiImage* bannerImage() const;

  /*!
   * \brief setBannerSize
   * \param size
   */
  void setBannerSize(uint32_t size);

  /*!
   * \brief bannerSize
   * \return
   */
  uint32_t bannerSize() const;

  /*!
   * \brief setTitle
   * \param title
   */
  void setTitle(const std::u16string& title);

  /*!
   * \brief title
   * \return
   */
  const std::u16string& title() const;

  /*!
   * \brief setSubtitle
   * \param subtitle
   */
  void setSubtitle(const std::u16string& subtitle);

  /*!
   * \brief subtitle
   * \return
   */
  const std::u16string& subtitle() const;

  /*!
   * \brief addIcon
   * \param icon
   */
  void addIcon(WiiImage* icon);

  /*!
   * \brief setIcon
   * \param id
   * \param icon
   */
  void setIcon(uint32_t id, WiiImage* icon);

  /*!
   * \brief getIcon
   * \param id
   * \return
   */
  WiiImage* getIcon(uint32_t id) const;

  /*!
   * \brief icons
   * \return
   */
  std::vector<WiiImage*> icons() const;

  /*!
   * \brief setAnimationSpeed
   * \param animSpeed
   */
  void setAnimationSpeed(uint16_t animSpeed);

  /*!
   * \brief animationSpeed
   * \return
   */
  uint16_t animationSpeed() const;

  /*!
   * \brief setPermissions
   * \param permissions
   */
  void setPermissions(uint8_t permissions);

  /*!
   * \brief permissions
   * \return
   */
  uint8_t permissions() const;

  /*!
   * \brief setFlags
   * \param flags
   */
  void setFlags(uint32_t flags);

  /*!
   * \brief flags
   * \return
   */
  uint32_t flags() const;

protected:
private:
  uint64_t m_gameId;
  WiiImage* m_banner;
  uint32_t m_animSpeed;
  uint8_t m_permissions;
  uint32_t m_flags;
  uint32_t m_bannerSize;
  std::vector<WiiImage*> m_icons;
  std::u16string m_title;
  std::u16string m_subtitle;
};
} // namespace athena
