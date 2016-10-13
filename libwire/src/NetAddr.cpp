#include "NetAddr.h"

#include <system_error>

#include "Utils.h"

namespace Net {

NetAddr::NetAddr(std::string const& addr)
: _addr(addr) {
}

NetAddr::NetAddr(struct sockaddr const* addr)
: _addr(addrStr(addr)) {
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

}
