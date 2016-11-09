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

enum class Version {
    V4,
    V6,
    UNKNOWN
};

enum class Transport {
    TCP,
    UDP,
    ICMP,
    UNKNOWN
};

//! @throw std::system_error
std::vector<Net::InterfaceInfo> listInterfaces();

}

#endif // NETUTILS_H
