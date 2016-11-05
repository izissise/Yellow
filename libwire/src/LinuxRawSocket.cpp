#include "LinuxRawSocket.h"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ether.h>

namespace Net {

uint8_t LinuxRawSocket::_sharedBuffer[LinuxRawSocket::buffSize];

LinuxRawSocket::LinuxRawSocket(std::function<void (data_t const& data)> readCallback)
: ARawSocket(readCallback) {
    _fd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE));
    if (_fd < 0) {
        throw lastSystemError();
    }
}

LinuxRawSocket::~LinuxRawSocket() {
    close(_fd);
}

void LinuxRawSocket::startSniffing(Net::InterfaceInfo const& interface) {
    auto iface = interface.getName().addr();
    if (setsockopt(_fd, SOL_SOCKET, SO_BINDTODEVICE, iface.c_str(), static_cast<socklen_t>(iface.size())) < 0) {
        auto err = lastSystemError();
        close(_fd);
        throw err;
    }
}

data_t LinuxRawSocket::_readSock() const {
   sockaddr_storage saddr;
   socklen_t saddrSize = sizeof(saddr);

   ssize_t dataSize = recvfrom(_fd, _sharedBuffer, sizeof(_sharedBuffer), 0, reinterpret_cast<sockaddr*>(&saddr), &saddrSize);
   if (dataSize < 0) {
       throw lastSystemError();
   }
   return data_t(_sharedBuffer, dataSize);
}

}
