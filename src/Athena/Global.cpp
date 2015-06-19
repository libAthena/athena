#include "Athena/Global.hpp"

std::ostream& operator<<(std::ostream& os, const Athena::SeekOrigin& origin)
{
    switch (origin)
    {
        case Athena::SeekOrigin::Begin:
            os << "Begin";
            break;

        case Athena::SeekOrigin::Current:
            os << "Current";
            break;

        case Athena::SeekOrigin::End:
            os << "End";
            break;
    }

    return os;
}


std::ostream& operator<<(std::ostream& os, const Athena::Endian& endian)
{
    switch (endian)
    {
        case Athena::Endian::LittleEndian:
            os << "LittleEndian";
            break;

        case Athena::Endian::BigEndian:
            os << "BigEndian";
            break;
    }

    return os;
}


static atEXCEPTION_HANDLER g_atExceptionHandler = nullptr;

atEXCEPTION_HANDLER atGetExceptionHandler()
{
    return g_atExceptionHandler;
}


void atSetExceptionHandler(atEXCEPTION_HANDLER func)
{
    g_atExceptionHandler = func;
}
