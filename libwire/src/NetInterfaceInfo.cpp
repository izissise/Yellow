#include "NetInterfaceInfo.h"


namespace Net {

InterfaceInfo::InterfaceInfo(struct ifaddrs const& ifaInfos)
: _name(ifaInfos.ifa_name), _addr(ifaInfos.ifa_addr),
  _netmask(ifaInfos.ifa_netmask), _broadcast(ifaInfos.ifa_dstaddr),
  _flags(ifaInfos.ifa_flags) {
}

}
