#include "athena/ALTTPFile.hpp"
#include "athena/ALTTPQuest.hpp"

namespace athena {
ALTTPFile::ALTTPFile() {}

ALTTPFile::ALTTPFile(std::vector<ALTTPQuest*> quests, std::vector<ALTTPQuest*> backup)
: m_quests(quests), m_backup(backup) {}

void ALTTPFile::setQuest(atUint32 id, ALTTPQuest* val) {
  if (id > m_quests.size()) {
    atWarning("index out of range");
    return;
  }

  m_quests[id] = val;
}

std::vector<ALTTPQuest*> ALTTPFile::questList() const { return m_quests; }
ALTTPQuest* ALTTPFile::quest(atUint32 id) const {
  if (id > m_quests.size()) {
    atWarning("index out of range");
    return nullptr;
  }

  return m_quests[id];
}

atUint32 ALTTPFile::questCount() const { return (atUint32)m_quests.size(); }
} // namespace athena
