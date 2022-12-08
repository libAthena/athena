#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Athena/Global.hpp"
#include "sockwrap.h"

namespace Athena
{
namespace net
{

class Socket
{
    // Disable copying
public:
    enum Type
    {
        TCP,
        UDP
    };

    explicit Socket(Type type = TCP);

    virtual ~Socket() { close() ; }

    void setBlocking(bool blocking);
    bool isBlocking() const { return m_isBlocking; }
protected:
    sockhandle_t handle() { return m_handle; }
    void create();
    void close();
private:
    // Disable copying
    Socket(const Socket&)=delete;
    Socket& operator=(const Socket&)=delete;
    Type         m_type;
    sockhandle_t m_handle;
    bool         m_isBlocking;
};

}
}

#endif
