#include "LinuxRawSocket.h"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

namespace Net {

uint8_t LinuxRawSocket::_sharedBuffer[LinuxRawSocket::buffSize];

LinuxRawSocket::LinuxRawSocket(Net::Version version, Net::Protocol protocol, std::function<void (data_t const& data)> readCallback)
: ARawSocket(readCallback) {
    _fd = socket(version, SOCK_RAW, protocol);
    if (_fd < 0) {
        throw lastSystemError();
    }
}

LinuxRawSocket::~LinuxRawSocket() {
    close(_fd);
}

void LinuxRawSocket::startSniffing(std::string const& interface) {
    if (setsockopt(_fd, SOL_SOCKET, SO_BINDTODEVICE, interface.c_str(), static_cast<socklen_t>(interface.size())) < 0) {
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
