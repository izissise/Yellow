#include "NetAddr.h"

#include <system_error>
#include <sys/socket.h>
#include <netdb.h>

#include "ScopeGuard.h"

namespace Net {

NetAddr::NetAddr::NetAddr()
: _addr(""), _version(Version::UNKNOWN) {
}

NetAddr::NetAddr(std::string const& addr)
: _addr(addr), _version(versionFromString(_addr)) {
}

NetAddr::NetAddr(const uint32_t& addr) {
    struct sockaddr_storage source;
    struct sockaddr_in* ipv4ptr;

    std::memset(&source, 0, sizeof(source));

    ipv4ptr = reinterpret_cast<struct sockaddr_in*>(&source);
    ipv4ptr->sin_addr.s_addr = addr;
    ipv4ptr->sin_family = AF_INET;
    addrSet(reinterpret_cast<struct sockaddr const*>(&source));
}

NetAddr::NetAddr(const in6_addr& addr) {
    struct sockaddr_storage source;
    struct sockaddr_in6* ipv6ptr;

    std::memset(&source, 0, sizeof(source));

    ipv6ptr = reinterpret_cast<struct sockaddr_in6*>(&source);
    ipv6ptr->sin6_addr = addr;
    ipv6ptr->sin6_family = AF_INET6;
    addrSet(reinterpret_cast<struct sockaddr const*>(&source));
}

NetAddr::NetAddr(struct sockaddr const* addr) {
    addrSet(addr);
}

void NetAddr::operator=(struct sockaddr const* addr) {
    addrSet(addr);
}

bool NetAddr::operator==(Net::NetAddr const& o) const {
   return _addr == o._addr;
}

bool NetAddr::operator!=(Net::NetAddr const& o) const {
    return !(_addr == o._addr);
}

void NetAddr::addrSet(struct sockaddr const* sa) {
    const void *res;
    char buff[BUFSIZ] = { 0 };

    if (sa == nullptr) {
        throw std::system_error(EINVAL, std::system_category());
    }
    res = nullptr;
    if (sa->sa_family == AF_INET) {
        res = reinterpret_cast<const void*>(&((reinterpret_cast<const struct sockaddr_in*>(sa))->sin_addr));
        _version = Version::V4;
    } else if (sa->sa_family == AF_INET6) {
        res = reinterpret_cast<const void*>(&((reinterpret_cast<const struct sockaddr_in6*>(sa))->sin6_addr));
        _version = Version::V6;
    } else {
        throw std::system_error(EINVAL, std::system_category());
    }
    if (inet_ntop(sa->sa_family, res, buff, sizeof(buff)) == nullptr) {
        throw lastSystemError();
    }
   _addr = buff;
}

Net::Version NetAddr::versionFromString(std::string const& addr) const {
    struct addrinfo hint, *res = nullptr;
    int ret;
    ScopeGuard sg([&res] () {
        freeaddrinfo(res);
    });

    std::memset(&hint, 0, sizeof hint);

    hint.ai_family = PF_UNSPEC;
    hint.ai_flags = AI_NUMERICHOST;
    ret = getaddrinfo(addr.c_str(), nullptr, &hint, &res);
    if (ret) {
        throw std::runtime_error("Not an ip address");
    }
    if(res->ai_family == AF_INET) {
        return Net::Version::V4;
    } else if (res->ai_family == AF_INET6) {
        return Net::Version::V6;
    }
    return Net::Version::UNKNOWN;
}

uint32_t NetAddr::addrGetRawV4() const {
    if (_version != Version::V4)
        throw std::runtime_error("Wrong addr type");
    sockaddr_storage st;
    addrGRaw(st);
    sockaddr_in* tmp = reinterpret_cast<sockaddr_in*>(&st);
    return tmp->sin_addr.s_addr;
}

in6_addr NetAddr::addrGetRawV6() const {
    if (_version != Version::V6)
        throw std::runtime_error("Wrong addr type");
    sockaddr_storage st;
    addrGRaw(st);
    sockaddr_in6* tmp = reinterpret_cast<sockaddr_in6*>(&st);
    return tmp->sin6_addr;
}

void NetAddr::addrGRaw(sockaddr_storage& store) const {
    int ret;
    int af;

    if (_version == Version::V4)
        af = AF_INET;
    else if (_version == Version::V6)
        af = AF_INET6;
    else
        throw std::runtime_error("Not a support network layer type.");
    ret = inet_pton(af, _addr.c_str(), &store);
    if (ret < 0)
        throw lastSystemError();
    else if (ret == 0)
        throw std::runtime_error("Not an ip address");
}

std::ostream& operator<<(std::ostream& f, Net::NetAddr const& addr) {
    f << addr.addr();
    return f;
}

}
