#include "athena/Socket.hpp"

#ifndef _WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cerrno>
#else
#include <WinSock2.h>
#include <Ws2tcpip.h>
#endif

namespace athena::net {

/* Define the low-level send/receive flags, which depend on the OS */
#ifdef __linux__
static const int _flags = MSG_NOSIGNAL;
#else
static const int _flags = 0;
#endif

void IPAddress::resolve(const std::string& address) {
  m_address = 0;
  m_valid = false;

  if (address == "255.255.255.255") {
    /* The broadcast address needs to be handled explicitly,
     * because it is also the value returned by inet_addr on error */
    m_address = INADDR_BROADCAST;
    m_valid = true;
  } else if (address == "0.0.0.0") {
    m_address = INADDR_ANY;
    m_valid = true;
  } else {
    /* Try to convert the address as a byte representation ("xxx.xxx.xxx.xxx") */
    struct in_addr addr;
    if (inet_pton(AF_INET, address.c_str(), &addr) == 1) {
      m_address = addr.s_addr;
      m_valid = true;
    } else {
      /* Not a valid address, try to convert it as a host name */
      addrinfo hints;
      memset(&hints, 0, sizeof(hints));
      hints.ai_family = AF_INET;
      addrinfo* result = NULL;
      if (getaddrinfo(address.c_str(), NULL, &hints, &result) == 0) {
        if (result) {
          addr = reinterpret_cast<sockaddr_in*>(result->ai_addr)->sin_addr;
          freeaddrinfo(result);
          m_address = addr.s_addr;
          m_valid = true;
        }
      }
    }
  }
}

uint32_t IPAddress::toInteger() const { return ntohl(m_address); }

static sockaddr_in createAddress(uint32_t address, unsigned short port) {
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_addr.s_addr = htonl(address);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

#ifdef __APPLE__
  addr.sin_len = sizeof(addr);
#endif

  return addr;
}

bool Socket::openSocket() {
  if (isOpen())
    return false;

  m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_socket == -1) {
    atError(fmt("Can't allocate socket"));
    return false;
  }

  int one = 1;
  setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&one), sizeof(one));
#ifdef __APPLE__
  setsockopt(m_socket, SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<char*>(&one), sizeof(one));
#endif

  setBlocking(m_isBlocking);

  return true;
}

void Socket::setRemoteSocket(int remSocket) {
  close();
  m_socket = remSocket;
  setBlocking(m_isBlocking);
}

#ifdef _WIN32
Socket::EResult Socket::LastWSAError() {
  switch (WSAGetLastError()) {
  case WSAEWOULDBLOCK:
  case WSAEALREADY:
    return EResult::Busy;
  default:
    return EResult::Error;
  }
}
#endif

void Socket::setBlocking(bool blocking) {
  m_isBlocking = blocking;
#ifndef _WIN32
  int status = fcntl(m_socket, F_GETFL);
  if (m_isBlocking)
    fcntl(m_socket, F_SETFL, status & ~O_NONBLOCK);
  else
    fcntl(m_socket, F_SETFL, status | O_NONBLOCK);
#else
  u_long b = blocking ? 0 : 1;
  ioctlsocket(m_socket, FIONBIO, &b);
#endif
}

bool Socket::openAndListen(const IPAddress& address, uint32_t port) {
  if (!openSocket())
    return false;

  sockaddr_in addr = createAddress(address.toInteger(), port);
  if (bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) == -1) {
    /* Not likely to happen, but... */
    atError(fmt("Failed to bind listener socket to port {}"), port);
    return false;
  }

  if (::listen(m_socket, 0) == -1) {
    /* Oops, socket is deaf */
    atError(fmt("Failed to listen to port {}"), port);
    return false;
  }

  return true;
}

Socket::EResult Socket::accept(Socket& remoteSocketOut, sockaddr_in& fromAddress) {
  if (!isOpen())
    return EResult::Error;

  /* Accept a new connection */
  socklen_t length = sizeof(sockaddr_in);
  int remoteSocket = ::accept(m_socket, reinterpret_cast<sockaddr*>(&fromAddress), &length);

  /* Check for errors */
  if (remoteSocket == -1) {
#ifndef _WIN32
    EResult res = (errno == EAGAIN) ? EResult::Busy : EResult::Error;
    if (res == EResult::Error)
      atError(fmt("Failed to accept incoming connection: {}"), strerror(errno));
#else
    EResult res = LastWSAError();
    if (res == EResult::Error)
      atError("Failed to accept incoming connection");
#endif
    return res;
  }

  /* Initialize the new connected socket */
  remoteSocketOut.setRemoteSocket(remoteSocket);

  return EResult::OK;
}

Socket::EResult Socket::accept(Socket& remoteSocketOut) {
  sockaddr_in fromAddress;
  return accept(remoteSocketOut, fromAddress);
}

Socket::EResult Socket::accept(Socket& remoteSocketOut, std::string& fromHostname) {
  sockaddr_in fromAddress;
  socklen_t len = sizeof(fromAddress);
  char name[NI_MAXHOST];
  EResult res = accept(remoteSocketOut, fromAddress);
  if (res == EResult::OK)
    if (getnameinfo((sockaddr*)&fromAddress, len, name, NI_MAXHOST, nullptr, 0, 0) == 0)
      fromHostname.assign(name);
  return res;
}

void Socket::close() {
  if (!isOpen())
    return;
#ifndef _WIN32
  ::close(m_socket);
#else
  closesocket(m_socket);
#endif
  m_socket = -1;
}

Socket::EResult Socket::send(const void* buf, size_t len, size_t& transferred) {
  transferred = 0;
  if (!isOpen())
    return EResult::Error;

  if (!buf || !len)
    return EResult::Error;

  /* Loop until every byte has been sent */
  int result = 0;
  for (size_t sent = 0; sent < len; sent += result) {
    /* Send a chunk of data */
    result = ::send(m_socket, static_cast<const char*>(buf) + sent, len - sent, _flags);

    /* Check for errors */
    if (result < 0)
#ifndef _WIN32
      return (errno == EAGAIN) ? EResult::Busy : EResult::Error;
#else
      return LastWSAError();
#endif
  }

  transferred = len;
  return EResult::OK;
}

Socket::EResult Socket::send(const void* buf, size_t len) {
  size_t transferred;
  return send(buf, len, transferred);
}

Socket::EResult Socket::recv(void* buf, size_t len, size_t& transferred) {
  transferred = 0;
  if (!isOpen())
    return EResult::Error;

  if (!buf)
    return EResult::Error;

  if (!len)
    return EResult::OK;

  /* Receive a chunk of bytes */
  int result = ::recv(m_socket, static_cast<char*>(buf), static_cast<int>(len), _flags);

  if (result < 0)
#ifndef _WIN32
    return (errno == EAGAIN) ? EResult::Busy : EResult::Error;
#else
    return LastWSAError();
#endif
  else if (result == 0)
    return EResult::Error;

  transferred = result;
  return EResult::OK;
}

Socket::EResult Socket::recv(void* buf, size_t len) {
  size_t transferred;
  return recv(buf, len, transferred);
}

} // namespace athena::net
