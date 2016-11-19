#ifndef NETADDR_H
#define NETADDR_H

#include <string>
#include <stdexcept>
#include <cstring>

#include <sys/types.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>

#include "Utils.h"
#include "NetType.h"

namespace Net {

class NetAddr {
public:
    NetAddr();

    NetAddr(std::string const& addr);

    //! @throw std::system_error
    NetAddr(uint32_t const& addr);
    //! @throw std::system_error
    NetAddr(in6_addr const& addr);

    //! @throw std::system_error
    NetAddr(struct sockaddr const* addr);
    //! @throw std::system_error
    void operator=(struct sockaddr const* addr);

    ~NetAddr() = default;

    Net::Version version() const { return _version; }
    std::string addr() const { return _addr; };

    bool operator==(Net::NetAddr const& o) const;
    bool operator!=(Net::NetAddr const& o) const;

    operator std::string() const { return addr(); };

    //! @throw std::system_error
    //! @throw std::runtime_error
    uint32_t addrGetRawV4() const;
    //! @throw std::system_error
    //! @throw std::runtime_error
    in6_addr addrGetRawV6() const;
protected:
    //! @throw std::system_error
    void addrSet(struct sockaddr const* addr);
    void addrGRaw(sockaddr_storage& store) const;
    Net::Version versionFromString(std::string const& addr) const;

protected:
    std::string _addr;
    Net::Version _version;

};

std::ostream& operator<<(std::ostream& f, Net::NetAddr const& addr);

}

#endif // NETADDR_H
