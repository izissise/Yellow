#include "NetInterfaceInfo.h"


namespace Net {

InterfaceInfo::InterfaceInfo(struct ifaddrs const& ifaInfos)
: _name(ifaInfos.ifa_name), _flags(ifaInfos.ifa_flags) {
    if (ifaInfos.ifa_addr)
        _addr = NetAddr(ifaInfos.ifa_addr);
    if (ifaInfos.ifa_netmask)
        _netmask = NetAddr(ifaInfos.ifa_netmask);
    if (ifaInfos.ifa_dstaddr)
        _broadcast = NetAddr(ifaInfos.ifa_dstaddr);
}

}
