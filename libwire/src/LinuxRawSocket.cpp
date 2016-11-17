#include "LinuxRawSocket.h"

#include <cstring>

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ether.h>

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
    if (setsockopt(_fd, SOL_SOCKET, SO_BINDTODEVICE, iface.c_str(), static_cast<socklen_t>(iface.size())) < 0) {
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
