#include "Athena/IPAddress.hpp"

#ifdef __unix__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#elif defined(_WIN32)
#ifdef _WIN32_WINDOWS
#undef _WIN32_WINDOWS
#endif
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINNT   0x0501
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

namespace Athena
{
namespace net
{
const IPAddress IPAddress::Any;
const IPAddress IPAddress::None      = IPAddress(  0,   0,   0,   0);
const IPAddress IPAddress::Localhost = IPAddress(127,   0,   0,   1);
const IPAddress IPAddress::Broadcast = IPAddress(255, 255, 255, 255);

IPAddress::IPAddress(const std::string& address)
    : m_valid(false)
{
    resolve(address);
}

IPAddress::IPAddress(atUint8 a, atUint8 b, atUint8 c, atUint8 d)
    : m_address(htonl((a << 24)| (b << 16) | (c << 8) | d)),
      m_valid(true)
{
}

IPAddress::IPAddress(atUint32 address)
    : m_address(htonl(address)),
      m_valid(true)
{
}

const std::string IPAddress::toString() const
{
    in_addr address;
    address.s_addr = m_address;
    return inet_ntoa(address);
}

void IPAddress::resolve(const std::string& address)
{
    if (address == "0.0.0.0")
    {
        m_address = 0;
        m_valid = true;
    }
    else if(address == "255.255.255.255")
    {
        m_address = ~0u;
        m_valid = true;
    }
    else
    {
        atUint32 ip = inet_addr(address.c_str());
        if (ip == INADDR_NONE)
        {
            addrinfo hints = {0};
            hints.ai_family = AF_INET;
            addrinfo* result = nullptr;
            if (getaddrinfo(address.c_str(), NULL, &hints, &result))
            {
                if (result)
                {
                    ip = reinterpret_cast<sockaddr_in*>(result->ai_addr)->sin_addr.s_addr;
                    freeaddrinfo(result);
                    m_address = ip;
                    m_valid = true;
                }
            }
        }
        else
        {
            m_address = ip;
            m_valid = true;
        }
    }
}
}
}
