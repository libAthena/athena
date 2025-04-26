#include "athena/WiiBanner.hpp"
#include "athena/WiiImage.hpp"

#include "athena/Utility.hpp"
#include <cstring>

namespace athena {

WiiBanner::WiiBanner() : m_gameId(0), m_banner(NULL), m_flags(0), m_bannerSize(0) {}

WiiBanner::WiiBanner(uint32_t gameId, const std::u16string& title, const std::u16string& subtitle, WiiImage* banner,
                     std::vector<WiiImage*> icons)
: m_gameId(gameId)
, m_banner(banner)
, m_flags(0)
, m_bannerSize(0)
, m_icons(icons)
, m_title(title)
, m_subtitle(subtitle) {}

WiiBanner::~WiiBanner() {
  delete m_banner;
  m_icons.clear();
}

void WiiBanner::setGameID(uint64_t id) { m_gameId = id; }

uint64_t WiiBanner::gameID() const { return m_gameId; }
void WiiBanner::setTitle(const std::u16string& title) { m_title = title; }

const std::u16string& WiiBanner::title() const { return m_title; }

void WiiBanner::setSubtitle(const std::u16string& subtitle) { m_subtitle = subtitle; }

const std::u16string& WiiBanner::subtitle() const { return m_subtitle; }

void WiiBanner::addIcon(WiiImage* icon) { m_icons.push_back(icon); }

void WiiBanner::setIcon(uint32_t id, WiiImage* icon) {
  if (m_icons[id] != NULL) {
    delete m_icons[id];
    m_icons[id] = icon;
  }
}

WiiImage* WiiBanner::getIcon(uint32_t id) const {
  if (!m_icons[id])
    return NULL;

  return m_icons[id];
}
std::vector<WiiImage*> WiiBanner::icons() const { return m_icons; }

void WiiBanner::setBannerImage(WiiImage* banner) { m_banner = banner; }

WiiImage* WiiBanner::bannerImage() const { return m_banner; }

void WiiBanner::setAnimationSpeed(uint16_t animSpeed) { m_animSpeed = animSpeed; }

uint16_t WiiBanner::animationSpeed() const { return m_animSpeed; }

void WiiBanner::setPermissions(uint8_t permissions) { m_permissions = permissions; }

uint8_t WiiBanner::permissions() const { return m_permissions; }

void WiiBanner::setBannerSize(uint32_t size) { m_bannerSize = size; }

uint32_t WiiBanner::bannerSize() const { return m_bannerSize; }

void WiiBanner::setFlags(uint32_t flags) { m_flags = flags; }

uint32_t WiiBanner::flags() const { return m_flags; }

} // namespace athena
