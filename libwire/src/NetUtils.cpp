#include "NetUtils.h"

#include <iostream>

#include "Utils.h"

namespace Net {

std::vector<Net::InterfaceInfo> listInterfaces() {
    std::vector<Net::InterfaceInfo> res;
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr)) {
        throw lastSystemError();
    }
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family == AF_PACKET)
            continue;
        try {
            res.push_back(InterfaceInfo(*ifa));
        } catch (std::system_error& err) {
            std::cerr << "Error: " << err.what() << " (" << err.code() << ")" << std::endl;
        }
    }
    freeifaddrs(ifaddr);
    return res;
}

}
