#include "Athena/Socket.hpp"

namespace Athena
{
namespace net
{

Socket::Socket(Socket::Type type)
    : m_type(type),
      m_handle(sock_invalid_socket()),
      m_isBlocking(true)
{}

void Socket::create()
{
    if (m_handle == sock_invalid_socket())
    {
        m_handle = socket(PF_INET, m_type == TCP ? SOCK_STREAM : SOCK_DGRAM, 0);
        setBlocking(m_isBlocking);

        int yes = 1;
        if (m_type == TCP)
        {
            // Disable Nagle algorithm
            if (setsockopt(m_handle, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int)) == -1)
                atWarning("Failed to set socket option \"TCP_NODELAY\", TCP packets will be buffered");

#ifdef __APPLE__
            if (setsockopt(m_handle, SOL_SOCKET, SO_NOSIGPIPE, &yes, sizeof(int)) == -1)
                atWarning("Failed to set socket option \"SO_NOSIGPIPE\"");
#endif
        }
        else
        {
            if (setsockopt(m_handle, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int)) == -1)
                atWarning("Failed to enable broadcast on UDP socket");
        }
    }
}

void Socket::close()
{
    sock_close_socket(m_handle);
    m_handle = sock_invalid_socket();
}

void Socket::setBlocking(bool blocking)
{
    sock_set_blocking(m_handle, blocking);
}

}
}
