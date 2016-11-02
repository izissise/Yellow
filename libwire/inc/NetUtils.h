#ifndef NETUTILS_H
#define NETUTILS_H

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "NetInterfaceInfo.h"
#include "ARawSocket.h"
#include "ANetwork.hpp"

namespace Net {

enum Version {
    V4 = AF_INET,
    V6 = AF_INET6
};

enum Protocol {
    TCP = IPPROTO_TCP,
    UDP = IPPROTO_UDP,
    ICMP = IPPROTO_ICMP,
    RAW = IPPROTO_RAW
};

//! @throw std::system_error
std::vector<Net::InterfaceInfo> listInterfaces();

//! @throw std::system_error
template<class SockType>
std::vector<std::shared_ptr<Net::ARawSocket>> sniffAllProtoIface(std::string const& iface, Net::ANetwork& net, std::function<void (data_t const& data)> cb) {
    std::vector<std::shared_ptr<Net::ARawSocket>> ret;
    for (auto proto : { Protocol::TCP, Protocol::UDP, Protocol::ICMP, Protocol::RAW }) {
        auto v4 = std::make_shared<SockType>(Net::Version::V4, proto, cb);
        net.registerRawSocket(v4);
        v4->startSniffing(iface);
        ret.push_back(v4);
        auto v6 = std::make_shared<SockType>(Net::Version::V6, proto, cb);
        net.registerRawSocket(v6);
        v6->startSniffing(iface);
        ret.push_back(v6);
    }
    return ret;
}

}

#endif // NETUTILS_H
