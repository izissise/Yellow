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
    NetAddr(std::string const& addr);
    NetAddr(struct sockaddr const* addr);

    ~NetAddr() = default;

    std::string addr() const { return _addr; };

    bool operator==(Net::NetAddr const& o) const;
    bool operator!=(Net::NetAddr const& o) const;

    operator std::string() const { return _addr; };

protected:
    static std::string addrStr(struct sockaddr const* addr);

protected:
    std::string _addr;

};

}

#endif // NETADDR_H
