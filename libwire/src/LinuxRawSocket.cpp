#include "LinuxRawSocket.h"

#include <cstring>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include <linux/if_packet.h>

#include "ScopeGuard.h"

namespace Net {

data_t LinuxRawSocket::_sharedBuffer;

LinuxRawSocket::LinuxRawSocket(std::function<void (data_t const& data)> readCallback)
: ARawSocket(readCallback) {
    _sharedBuffer.resize(buffSize);
    _fd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE));
    if (_fd < 0) {
        throw lastSystemError();
    }
}

LinuxRawSocket::~LinuxRawSocket() {
    close(_fd);
}

void LinuxRawSocket::startSniffing(Net::InterfaceInfo const& interface, bool promiscuous) {
    auto iface = interface.getName();
    struct sockaddr_ll sll;
    struct ifreq ifr;
    ifreq ifopts;
    int sockopt = 0;
    ScopeGuard sg([this] () { close(_fd); });

    if (promiscuous) {
        std::strncpy(ifopts.ifr_name, iface.c_str(), IFNAMSIZ - 1);
        if (ioctl(_fd, SIOCGIFFLAGS, &ifopts) < 0)
            throw lastSystemError();
        ifopts.ifr_flags |= IFF_PROMISC;
        if (ioctl(_fd, SIOCSIFFLAGS, &ifopts) < 0)
            throw lastSystemError();
    }
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(sockopt)) < 0)
        throw lastSystemError();

    std::memset(&sll, 0, sizeof(sll));
    std::memset(&ifr, 0, sizeof(ifr));
    std::strncpy(ifr.ifr_name, iface.c_str(), IFNAMSIZ);
    if((ioctl(_fd, SIOCGIFINDEX, &ifr)) == -1) {
        throw lastSystemError();
    }
    /* Bind our raw socket to this interface */
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_ALL);
    if((bind(_fd, reinterpret_cast<sockaddr*>(&sll), sizeof(sll))) == -1) {
        throw lastSystemError();
    }
    sg.deactivate();
}

data_t LinuxRawSocket::_readSock() const {

   ssize_t dataSize = recvfrom(_fd, _sharedBuffer.data(), _sharedBuffer.size(), 0, nullptr, nullptr);
   if (dataSize < 0 && errno != EINTR) {
       throw lastSystemError();
   }
   return subData(_sharedBuffer, dataSize);
}

}
