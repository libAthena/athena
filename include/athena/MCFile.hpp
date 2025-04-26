#pragma once

#include "athena/Global.hpp"

namespace athena {

class MCSlot;
/*! \class MCFile
 *  \brief The Minish Cap data container class class
 *
 * Contains all relevant data for a The Minish Cap save,
 * file.
 */
class MCFile {
public:
  static const char VERSION_EU_JP[33];
  static const char VERSION_US[33];
  enum SlotType { New = 0x54494E49, Valid = 0x4D435A33, Deleted = 0x466C6544 };

  MCFile();

  static uint8_t* unscramble(uint8_t* data, uint64_t length);

private:
  MCSlot* m_slots[3];
};

} // namespace athena
