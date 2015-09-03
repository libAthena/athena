#ifndef MCFILE_HPP
#define MCFILE_HPP

#include "Athena/Global.hpp"

namespace Athena
{

class MCSlot;
/*! \class MCFile
 *  \brief The Minish Cap data container class class
 *
 * Contains all relevant data for a The Minish Cap save,
 * file.
 */
class MCFile
{
public:
    static const char VERSION_EU_JP[33];
    static const char VERSION_US[33];
    enum SlotType
    {
        New     = 0x54494E49,
        Valid   = 0x4D435A33,
        Deleted = 0x466C6544
    };

    MCFile();

    static atUint8* unscramble(atUint8* data, atUint64 length);
private:
    MCSlot* m_slots[3];
};

} // zelda
#endif // MCFILE_HPP
