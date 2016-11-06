#include "NetAddr.h"

#include <system_error>

#include "Utils.h"

namespace Net {

NetAddr::NetAddr::NetAddr()
: _addr("") {
}

NetAddr::NetAddr(std::string const& addr)
: _addr(addr) {
}

NetAddr::NetAddr(const uint32_t& addr) {
    struct sockaddr_storage source;
    struct sockaddr_in* ipv4ptr;

    std::memset(&source, 0, sizeof(source));

    ipv4ptr = reinterpret_cast<struct sockaddr_in*>(&source);
    ipv4ptr->sin_addr.s_addr = addr;
    ipv4ptr->sin_family = AF_INET;
    _addr = addrStr(reinterpret_cast<struct sockaddr const*>(&source));
}

NetAddr::NetAddr(const in6_addr& addr) {
    struct sockaddr_storage source;
    struct sockaddr_in6* ipv6ptr;

    std::memset(&source, 0, sizeof(source));

    ipv6ptr = reinterpret_cast<struct sockaddr_in6*>(&source);
    ipv6ptr->sin6_addr = addr;
    ipv6ptr->sin6_family = AF_INET6;
    _addr = addrStr(reinterpret_cast<struct sockaddr const*>(&source));
}

NetAddr::NetAddr(struct sockaddr const* addr)
: _addr(addrStr(addr)) {
}

void NetAddr::operator=(struct sockaddr const* addr) {
    _addr = addrStr(addr);
}

bool NetAddr::operator==(Net::NetAddr const& o) const {
   return _addr == o._addr;
}

bool NetAddr::operator!=(Net::NetAddr const& o) const {
    return !(_addr == o._addr);
}

std::string NetAddr::addrStr(struct sockaddr const* sa) {
    const void *res;
    char buff[BUFSIZ] = { 0 };

    if (sa == nullptr) {
      return "";
    }
    res = nullptr;
    if (sa->sa_family == AF_INET)
        res = reinterpret_cast<const void*>(&((reinterpret_cast<const struct sockaddr_in*>(sa))->sin_addr));
    else if (sa->sa_family == AF_INET6)
        res = reinterpret_cast<const void*>(&((reinterpret_cast<const struct sockaddr_in6*>(sa))->sin6_addr));
    else
        throw std::system_error(EINVAL, std::system_category());
    if (inet_ntop(sa->sa_family, res, buff, sizeof(buff)) == nullptr) {
        throw lastSystemError();
    }
    return std::string(buff);
}

std::ostream& operator<<(std::ostream& f, Net::NetAddr const& addr) {
    f << addr.addr();
    return f;
}

}
