#include "athena/ALTTPFile.hpp"
#include "athena/ALTTPQuest.hpp"

namespace athena {
ALTTPFile::ALTTPFile() {}

ALTTPFile::ALTTPFile(std::vector<ALTTPQuest*> quests, std::vector<ALTTPQuest*> backup)
: m_quests(quests), m_backup(backup) {}

void ALTTPFile::setQuest(uint32_t id, ALTTPQuest* val) {
  if (id > m_quests.size()) {
    atWarning("index out of range");
    return;
  }

  m_quests[id] = val;
}

std::vector<ALTTPQuest*> ALTTPFile::questList() const { return m_quests; }
ALTTPQuest* ALTTPFile::quest(uint32_t id) const {
  if (id > m_quests.size()) {
    atWarning("index out of range");
    return nullptr;
  }

  return m_quests[id];
}

uint32_t ALTTPFile::questCount() const { return (uint32_t)m_quests.size(); }
} // namespace athena
