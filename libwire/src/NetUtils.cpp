#include "NetUtils.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>

#include "Utils.h"

namespace Net {

std::vector<Net::InterfaceInfo> listInterfaces() {
    std::vector<Net::InterfaceInfo> res;
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr)) {
        throw lastSystemError();
    }
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
#ifndef __APPLE__
        if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family == AF_PACKET)
            continue;
#endif
        try {
            res.push_back(InterfaceInfo(*ifa));
        } catch (std::system_error& err) {
            std::cerr << "Error: " << err.what() << " (" << err.code() << ")" << std::endl;
        }
    }
    freeifaddrs(ifaddr);
    return res;
}

void rawMacAddr(unsigned char buff[6], std::string const& mac) {
    unsigned int tmpBuff[6];
    char c;
    if (std::sscanf(mac.c_str(),
        "%x:%x:%x:%x:%x:%x%c",
        &tmpBuff[0], &tmpBuff[1], &tmpBuff[2],
        &tmpBuff[3], &tmpBuff[4], &tmpBuff[5], &c) != 6) {
            throw std::runtime_error(mac + std::string(" is an invalid MAC address"));
        }
    for (size_t i = 0; i < 6; ++i) {
        buff[i] = static_cast<uint8_t>(tmpBuff[i]);
    }
}

}
