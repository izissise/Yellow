#ifndef NETADDR_H
#define NETADDR_H

#include <string>
#include <stdexcept>

#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

namespace Net {

class NetAddr {
public:
    NetAddr();

    NetAddr(std::string const& addr);

    //! @throw std::system_error
    NetAddr(struct sockaddr const* addr);
    //! @throw std::system_error
    void operator=(struct sockaddr const* addr);

    ~NetAddr() = default;

    std::string addr() const { return _addr; };

    bool operator==(Net::NetAddr const& o) const;
    bool operator!=(Net::NetAddr const& o) const;

    operator std::string() const { return addr(); };

protected:
    //! @throw std::system_error
    static std::string addrStr(struct sockaddr const* addr);

protected:
    std::string _addr;

};

std::ostream& operator<<(std::ostream& f, Net::NetAddr const& addr);

}

#endif // NETADDR_H
