#ifndef NETINTERFACEINFO_H
#define NETINTERFACEINFO_H

#include <string>
#include <system_error>

#include <sys/types.h>
#include <ifaddrs.h>

#include "NetAddr.h"

namespace Net {

class InterfaceInfo {
public:
    explicit InterfaceInfo(struct ifaddrs const& ifaInfos);
    ~InterfaceInfo() = default;

    Net::NetAddr getName() const { return _name; };
    Net::NetAddr getAddr() const { return _addr; };
    Net::NetAddr getNetmask() const { return _netmask; };
    Net::NetAddr getBroadcast() const { return _broadcast; };

private:
    NetAddr _name;
    NetAddr _addr;
    NetAddr _netmask;
    NetAddr _broadcast;
    unsigned int _flags;
};

}

#endif // NETINTERFACEINFO_H
