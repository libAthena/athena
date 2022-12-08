#include "sockwrap.h"

#ifndef _WIN32
#include <errno.h>
#include <fcntl.h>
#endif

struct sockaddr_in sock_create_address(uint32_t address, uint16_t port)
{
    struct sockaddr_in addr = {0};
    addr.sin_addr.s_addr = htonl(address);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

#ifdef __APPLE__
    addr.sin_len = sizeof(addr);
#endif

    return addr;
}

void sock_close_socket(sockhandle_t sock)
{
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}

void sock_set_blocking(sockhandle_t sock, bool block)
{
#ifdef _WIN32
    u_long blocking = block ? 0 : 1;
    ioctlsocket(sock, FIONBIO, &blocking);
#else
    int status = fcntl(sock, F_GETFL);
    if (block)
        status &= ~O_NONBLOCK;
    else
        status |= O_NONBLOCK;

    fcntl(sock, F_SETFL, status);
#endif
}

sockstatus_t sock_error_status()
{
#ifdef _WIN32
    switch(WSAGetLastError())
    {
        case WSAEWOULDBLOCK:
        case WSAEALREADY:
            return SS_NotReady;
        case WSAECONNABORTED:
        case WSAECONNRESET:
        case WSATIMEDOUT:
        case WSAENETRESET:
        case WSAENOTCONN:
            return SS_Disconnected;
        case WSAEISCONN:
            return SS_Done;
        default:
            return SS_Error;
    }
#else
    if (errno == EAGAIN || errno == EINPROGRESS)
        return SS_NotReady;

    switch(errno)
    {
        case EWOULDBLOCK: return SS_NotReady;
        case ECONNABORTED:
        case ECONNRESET:
        case ETIMEDOUT:
        case ENETRESET:
        case ENOTCONN:
        case EPIPE:
            return SS_Disconnected;
        default:
            return SS_Error;
    }
#endif
}

sockhandle_t sock_invalid_socket()
{
#if _WIN32
    return INVALID_SOCKET;
#else
    return -1;
#endif
}
