#ifndef WIIBANNER_H
#define WIIBANNER_H

#include <vector>
#include <string>
#include "athena/Global.hpp"

namespace athena
{
class WiiImage;
/*! \class WiiBanner
 *  \brief Wii banner container class
 *
 * Contains all relevant data for a Wii banner.
 */
class WiiBanner
{
public:
    enum
    {
        NoCopy = 0x00000001,
        Bounce = 0x00000010,
        NoCopyBounce = NoCopy | Bounce
    };

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
    WiiBanner(atUint32 gameId, const std::string& title, const std::string& subtitle, WiiImage* m_banner, std::vector<WiiImage*> icons);
    virtual ~WiiBanner();

    /*!
     * \brief setGameID
     * \param id
     */
    void setGameID(atUint64 id);

    /*!
     * \brief gameID
     * \return
     */
    atUint64  gameID() const;

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
    void setBannerSize(atUint32 size);

    /*!
     * \brief bannerSize
     * \return
     */
    atUint32  bannerSize() const;

    /*!
     * \brief setTitle
     * \param title
     */
    void setTitle(const std::string& title);

    /*!
     * \brief title
     * \return
     */
    std::string title() const;

    /*!
     * \brief setSubtitle
     * \param subtitle
     */
    void setSubtitle(const std::string& subtitle);

    /*!
     * \brief subtitle
     * \return
     */
    std::string subtitle() const;

    /*!
     * \brief addIcon
     * \param icon
     */
    void      addIcon(WiiImage* icon);

    /*!
     * \brief setIcon
     * \param id
     * \param icon
     */
    void      setIcon(atUint32 id, WiiImage* icon);

    /*!
     * \brief getIcon
     * \param id
     * \return
     */
    WiiImage* getIcon(atUint32 id) const;

    /*!
     * \brief icons
     * \return
     */
    std::vector<WiiImage*> icons() const;

    /*!
     * \brief setAnimationSpeed
     * \param animSpeed
     */
    void setAnimationSpeed(atUint16 animSpeed);

    /*!
     * \brief animationSpeed
     * \return
     */
    atUint16  animationSpeed() const;

    /*!
     * \brief setPermissions
     * \param permissions
     */
    void setPermissions(atUint8 permissions);

    /*!
     * \brief permissions
     * \return
     */
    atUint8   permissions() const;

    /*!
     * \brief setFlags
     * \param flags
     */
    void setFlags(atUint32 flags);

    /*!
     * \brief flags
     * \return
     */
    atUint32  flags() const;
protected:
private:
    atUint64                 m_gameId;
    WiiImage*              m_banner;
    atUint32                 m_animSpeed;
    atUint8                  m_permissions;
    atUint32                 m_flags;
    atUint32                 m_bannerSize;
    std::vector<WiiImage*> m_icons;
    std::string            m_title;
    std::string            m_subtitle;
};
} // zelda

#endif // WIIBANNER_H
