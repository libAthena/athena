#ifndef IPADDRESS_HPP
#define IPADDRESS_HPP

#include "Athena/Global.hpp"
#include <string.h>

#include <cstring>
#include <utility>

namespace Athena
{
namespace net
{
class IPAddress
{
public:

    static const IPAddress None;
    static const IPAddress Any;
    static const IPAddress Localhost;
    static const IPAddress Broadcast;

    IPAddress() : m_address(~0u), m_valid(false) {}

    IPAddress(const std::string& address);

    IPAddress(atUint8 a, atUint8 b, atUint8 c, atUint8 d);

    IPAddress(atUint32 address);

    const std::string toString() const;
    const atUint32 toInt() const;

    static IPAddress localAddress();
private:

    atUint32 m_address;
    bool m_valid;
    void resolve(const std::string& address);
};
}
}

#endif
