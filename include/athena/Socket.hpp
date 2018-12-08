#pragma once

#include <sys/types.h>
#include <fcntl.h>
#include <string>
#include <memory.h>

#include "Global.hpp"

#ifdef _WIN32
#include <BaseTsd.h>
typedef UINT_PTR SOCKET;
#endif

struct sockaddr_in;

namespace athena::net {

/** IP address class derived from SFML */
class IPAddress {
  uint32_t m_address = 0;
  bool m_valid = false;

  void resolve(const std::string& address);

public:
  IPAddress(const std::string& address) { resolve(address); }

  uint32_t toInteger() const;
  operator bool() const { return m_valid; }
};

/** Server-oriented TCP socket class derived from SFML */
class Socket {
#ifndef _WIN32
  using SocketTp = int;
#else
  using SocketTp = SOCKET;
#endif
  SocketTp m_socket = -1;
  bool m_isBlocking;

  bool openSocket();
  void setRemoteSocket(int remSocket);

public:
  enum class EResult { OK, Error, Busy };

#ifdef _WIN32
  static EResult LastWSAError();
#endif

  Socket(bool blocking) : m_isBlocking(blocking) {}
  ~Socket() { close(); }

  Socket(const Socket& other) = delete;
  Socket& operator=(const Socket& other) = delete;
  Socket(Socket&& other) : m_socket(other.m_socket), m_isBlocking(other.m_isBlocking) { other.m_socket = -1; }
  Socket& operator=(Socket&& other) {
    close();
    m_socket = other.m_socket;
    other.m_socket = -1;
    m_isBlocking = other.m_isBlocking;
    return *this;
  }

  void setBlocking(bool blocking);
  bool isOpen() const { return m_socket != -1; }
  bool openAndListen(const IPAddress& address, uint32_t port);
  EResult accept(Socket& remoteSocketOut, sockaddr_in& fromAddress);
  EResult accept(Socket& remoteSocketOut);
  EResult accept(Socket& remoteSocketOut, std::string& fromHostname);
  void close();
  EResult send(const void* buf, size_t len, size_t& transferred);
  EResult send(const void* buf, size_t len);
  EResult recv(void* buf, size_t len, size_t& transferred);
  EResult recv(void* buf, size_t len);

  operator bool() const { return isOpen(); }

  SocketTp GetInternalSocket() const { return m_socket; }
};

} // namespace athena::net
