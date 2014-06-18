#ifndef ATHENA_NO_SAVES
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

#include "Athena/WiiBanner.hpp"
#include "Athena/WiiImage.hpp"

#include "Athena/Utility.hpp"
#include <string.h>

namespace Athena
{

WiiBanner::WiiBanner() :
    m_gameId(0),
    m_banner(NULL),
    m_flags(0),
    m_bannerSize(0),
    m_title(""),
    m_subtitle("")
{
}

WiiBanner::WiiBanner(atUint32 gameId, const std::string& title,
                     const std::string& subtitle, WiiImage* banner, std::vector<WiiImage*> icons) :
    m_gameId(gameId),
    m_banner(banner),
    m_flags(0),
    m_bannerSize(0),
    m_icons(icons),
    m_title(title),
    m_subtitle(subtitle)
{
}

WiiBanner::~WiiBanner()
{
    delete m_banner;
    m_icons.clear();
}

void WiiBanner::setGameID(atUint64 id)
{
    m_gameId = id;
}

atUint64 WiiBanner::gameID() const
{
    return m_gameId;
}
void WiiBanner::setTitle(const std::string& title)
{
    m_title = title;
}

std::string WiiBanner::title() const
{
    return m_title;
}

void WiiBanner::setSubtitle(const std::string& subtitle)
{
    m_subtitle = subtitle;
}

std::string WiiBanner::subtitle() const
{
    return m_subtitle;
}

void WiiBanner::addIcon(WiiImage* icon)
{
    m_icons.push_back(icon);
}

void WiiBanner::setIcon(atUint32 id, WiiImage* icon)
{
    if (m_icons[id] != NULL)
    {
        delete m_icons[id];
        m_icons[id] = icon;
    }
}

WiiImage* WiiBanner::getIcon(atUint32 id) const
{
    if (!m_icons[id])
        return NULL;

    return m_icons[id];
}
std::vector<WiiImage*> WiiBanner::icons() const
{
    return m_icons;
}

void WiiBanner::setBannerImage(WiiImage* banner)
{
    m_banner = banner;
}

WiiImage* WiiBanner::bannerImage() const
{
    return m_banner;
}

void WiiBanner::setAnimationSpeed(atUint16 animSpeed)
{
    m_animSpeed = animSpeed;
}

atUint16 WiiBanner::animationSpeed() const
{
    return m_animSpeed;
}

void WiiBanner::setPermissions(atUint8 permissions)
{
    m_permissions = permissions;
}

atUint8 WiiBanner::permissions() const
{
    return m_permissions;
}

void WiiBanner::setBannerSize(atUint32 size)
{
    m_bannerSize = size;
}

atUint32 WiiBanner::bannerSize() const
{
    return m_bannerSize;
}

void WiiBanner::setFlags(atUint32 flags)
{
    m_flags = flags;
}

atUint32 WiiBanner::flags() const
{
    return m_flags;
}

} // zelda
#endif // ATHENA_NO_SAVES
