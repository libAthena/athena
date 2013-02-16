// This file is part of libZelda.
//
// libZelda is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// libZelda is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with libZelda.  If not, see <http://www.gnu.org/licenses/>
#ifndef WIIBANNER_H
#define WIIBANNER_H


#include <vector>
#include <string>
#include <Types.hpp>

class WiiImage
{
public:
    WiiImage();
    WiiImage(Uint32 width, Uint32 height, Uint8* data);
    ~WiiImage();

    void setWidth(const Uint32 width);
    Uint32  width() const;

    void  setHeight(const Uint32 height);
    Uint32   height() const;

    void  setData(const Uint8* data);
    Uint8*   data();

    Uint8* toRGBA32();

private:
    Uint32 m_width;
    Uint32 m_height;
    Uint8* m_data;
};

class WiiBanner
{
public:
    enum { NoCopy = 0x00000001, Bounce = 0x00000010, NoCopyBounce = NoCopy | Bounce };
    WiiBanner();
    WiiBanner(Uint32 gameId, const std::string& title, const std::string& subtitle, WiiImage* m_banner, std::vector<WiiImage*> icons);
    virtual ~WiiBanner();

    void setGameID(Uint64 id);
    Uint64  gameID() const;

    void setBannerImage(WiiImage* banner);
    WiiImage* bannerImage() const;

    void setBannerSize(Uint32 size);
    Uint32  bannerSize() const;

    void setTitle(const std::string& title);
    std::string title() const;

    void     setSubtitle(const std::string& subtitle);
    std::string subtitle() const;

    void      addIcon(WiiImage* icon);
    void      setIcon(Uint32 id, WiiImage* icon);
    WiiImage* getIcon(Uint32 id) const;
    std::vector<WiiImage*> icons() const;

    void setAnimationSpeed(Uint16 animSpeed);
    Uint16  animationSpeed() const;

    void setPermissions(Uint8 permissions);
    Uint8   permissions() const;

    void setFlags(Uint32 flags);
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

#endif // WIIBANNER_H
