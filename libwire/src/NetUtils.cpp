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
