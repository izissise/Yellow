#ifndef NETINTERFACEINFO_H
#define NETINTERFACEINFO_H

#include <string>
#include <system_error>

#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include "NetAddr.h"

namespace Net {

class InterfaceInfo {
public:

    //! @throw std::system_error
    explicit InterfaceInfo(struct ifaddrs const& ifaInfos);
    ~InterfaceInfo() = default;

    Net::NetAddr getName() const { return _name; };
    Net::NetAddr getAddr() const { return _addr; };
    Net::NetAddr getNetmask() const { return _netmask; };
    Net::NetAddr getBroadcast() const { return _broadcast; };

    //Flag infos
    bool isUp() const { return ((_flags & IFF_UP) == IFF_UP); }; //Interface is running.
    bool hasValidBroadcast() const { return ((_flags & IFF_BROADCAST) == IFF_BROADCAST); }; //Valid broadcast address set.
    bool hasDebug() const { return ((_flags & IFF_DEBUG) == IFF_DEBUG); }; //Internal debugging flag.
    bool isLoopback() const { return ((_flags & IFF_LOOPBACK) == IFF_LOOPBACK); }; //Interface is a loopback interface.
    bool isP2p() const { return ((_flags & IFF_POINTOPOINT) == IFF_POINTOPOINT); }; //Interface is a point-to-point link.
    bool isRunning() const { return ((_flags & IFF_RUNNING) == IFF_RUNNING); }; //Resources allocated.
    bool noArp() const { return ((_flags & IFF_NOARP) == IFF_NOARP); }; //No arp protocol, L2 destination address not set.
    bool inPromiscuous() const { return ((_flags & IFF_PROMISC) == IFF_PROMISC); }; //Interface is in promiscuous mode.
    bool noTrailers() const { return ((_flags & IFF_NOTRAILERS) == IFF_NOTRAILERS); }; //Avoid use of trailers.
    bool receiveMulticast() const { return ((_flags & IFF_ALLMULTI) == IFF_ALLMULTI); }; //Receive all multicast packets.
//    bool isBundleMaster() const { return ((_flags & IFF_MASTER) == IFF_MASTER); }; //Master of a load balancing bundle.
//    bool isBundleSlave() const { return ((_flags & IFF_SLAVE) == IFF_SLAVE); }; //Slave of a load balancing bundle.
    bool hasMulticast() const { return ((_flags & IFF_MULTICAST) == IFF_MULTICAST); }; //Supports multicast
//    bool hasMediaType() const { return ((_flags & IFF_PORTSEL) == IFF_PORTSEL); }; //Is able to select media type via ifmap.
//    bool isAutoMediaType() const { return ((_flags & IFF_AUTOMEDIA) == IFF_AUTOMEDIA); }; //Auto media selection active.
//    bool isDynamic() const { return ((_flags & IFF_DYNAMIC) == IFF_DYNAMIC); }; //The addresses are lost when the interface goes down.

protected:
    NetAddr _name;
    NetAddr _addr;
    NetAddr _netmask;
    NetAddr _broadcast;
    unsigned int _flags;
};

}

#endif // NETINTERFACEINFO_H
