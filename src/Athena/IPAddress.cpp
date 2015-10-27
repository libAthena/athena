#include "Athena/IPAddress.hpp"
#include "sockwrap.h" // To resolve local IP

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

const atUint32 IPAddress::toInt() const
{
    return ntohl(m_address);
}

IPAddress IPAddress::localAddress()
{
    sockhandle_t sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == sock_invalid_socket())
        return IPAddress();

    struct sockaddr_in address = sock_create_address(ntohl(INADDR_LOOPBACK), 9);
    if (connect(sock, reinterpret_cast<sockaddr*>(&address), sizeof(struct sockaddr_in)) == -1)
    {
        sock_close_socket(sock);
        return IPAddress();
    }

    addrlen_t size = sizeof(address);
    if (getsockname(sock, reinterpret_cast<sockaddr*>(&address), &size) == -1)
    {
        sock_close_socket(sock);
        return IPAddress();
    }

    sock_close_socket(sock);
    return IPAddress(ntohl(address.sin_addr.s_addr));
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
            addrinfo hints;
            memset(&hints, 0, sizeof(addrinfo));
            hints.ai_family = AF_INET;
            addrinfo* result = nullptr;
            if (getaddrinfo(address.c_str(), nullptr, &hints, &result) == 0)
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
