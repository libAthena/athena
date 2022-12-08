#ifndef SOCKWRAP_H
#define SOCKWRAP_H

#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
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
#include <basetsd>

typedef UINT_PTR sockhandle_t;
typedef int      addrlen_t;
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

typedef int32_t   sockhandle_t;
typedef socklen_t addrlen_t;
#endif

#define ANY_PORT 0

#ifdef __cplusplus
extern "C" {
#endif

// TODO: More granular errors
typedef enum
{
    SS_Done,
    SS_NotReady,
    SS_Partial,
    SS_Disconnected,
    SS_Error
} sockstatus_t;

struct sockaddr_in sock_create_address(uint32_t address, uint16_t port);
void sock_close_socket(sockhandle_t sock);
void sock_set_blocking(sockhandle_t sock, bool block);
sockstatus_t sock_error_status();
sockhandle_t sock_invalid_socket();

#ifdef __cplusplus
}
#endif

#endif
