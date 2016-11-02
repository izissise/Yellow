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

//! @throw std::system_error
std::vector<Net::InterfaceInfo> listInterfaces();

//! @throw std::system_error
template<class SockType>
std::vector<std::shared_ptr<Net::ARawSocket>> sniffAllProtoIface(std::string const& iface, Net::ANetwork& net, std::function<void (data_t const& data)> cb) {
    std::vector<std::shared_ptr<Net::ARawSocket>> ret;
    for (auto proto : { ARawSocket::Protocol::TCP, ARawSocket::Protocol::UDP, ARawSocket::Protocol::ICMP, ARawSocket::Protocol::RAW }) {
        auto v4 = std::make_shared<SockType>(Net::ARawSocket::Version::V4, proto, cb);
        net.registerRawSocket(v4);
        v4->startSniffing(iface);
        ret.push_back(v4);
        auto v6 = std::make_shared<SockType>(Net::ARawSocket::Version::V6, proto, cb);
        net.registerRawSocket(v6);
        v6->startSniffing(iface);
        ret.push_back(v6);
    }
    return ret;
}

}

#endif // NETUTILS_H
