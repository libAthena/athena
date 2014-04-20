// This file is part of libAthena.
//
// libAthena is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libAthena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libAthena.  If not, see <http://www.gnu.org/licenses/>

#ifndef WIIBANNER_H
#define WIIBANNER_H


#include <vector>
#include <string>
#include "Athena/Global.hpp"

namespace Athena
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
    WiiBanner(Uint32 gameId, const std::string& title, const std::string& subtitle, WiiImage* m_banner, std::vector<WiiImage*> icons);
    virtual ~WiiBanner();

    /*!
     * \brief setGameID
     * \param id
     */
    void setGameID(Uint64 id);

    /*!
     * \brief gameID
     * \return
     */
    Uint64  gameID() const;

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
    void setBannerSize(Uint32 size);

    /*!
     * \brief bannerSize
     * \return
     */
    Uint32  bannerSize() const;

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
    void      setIcon(Uint32 id, WiiImage* icon);

    /*!
     * \brief getIcon
     * \param id
     * \return
     */
    WiiImage* getIcon(Uint32 id) const;

    /*!
     * \brief icons
     * \return
     */
    std::vector<WiiImage*> icons() const;

    /*!
     * \brief setAnimationSpeed
     * \param animSpeed
     */
    void setAnimationSpeed(Uint16 animSpeed);

    /*!
     * \brief animationSpeed
     * \return
     */
    Uint16  animationSpeed() const;

    /*!
     * \brief setPermissions
     * \param permissions
     */
    void setPermissions(Uint8 permissions);

    /*!
     * \brief permissions
     * \return
     */
    Uint8   permissions() const;

    /*!
     * \brief setFlags
     * \param flags
     */
    void setFlags(Uint32 flags);

    /*!
     * \brief flags
     * \return
     */
    Uint32  flags() const;
protected:
private:
    Uint64                 m_gameId;
    WiiImage*              m_banner;
    Uint32                 m_animSpeed;
    Uint8                  m_permissions;
    Uint32                 m_flags;
    Uint32                 m_bannerSize;
    std::vector<WiiImage*> m_icons;
    std::string            m_title;
    std::string            m_subtitle;
};
} // zelda

#endif // WIIBANNER_H
