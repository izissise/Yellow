#include "LinuxPacketReader.h"

#include "Packet.h"

namespace Net {

LinuxPacketReader::LinuxPacketReader()
: _buffer(std::make_unique<char[]>(65536)) {
}

int setSocket(std::string interface, int protocol) {
  int fd = socket(AF_INET, SOCK_RAW, protocol);
  if (fd < 0) {
    throw lastSystemError();
    return fd;
  }
  setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, interface.c_str(), 4);
  return fd;
}

void LinuxPacketReader::startListening(Net::InterfaceInfo const& interface) {
    (void)interface;
    sockets[0] = setSocket(interface.getName(), IPPROTO_TCP);
    sockets[1] = setSocket(interface.getName(), IPPROTO_UDP);
    sockets[2] = setSocket(interface.getName(), IPPROTO_ICMP);
    FD_ZERO(&readset);

    int fd_max = 0;
    for (int i = 0; i < 3; i++) {
        int socket = sockets[i];
        if (socket < 0) {
            std::cout << "Socket Error" << std::endl;
	    throw lastSystemError();
        }
        if (socket > fd_max) {
            fd_max = socket;
        }
        std::cout << "add socket fd : " << socket << std::endl;
        FD_SET(socket, &readset);
    }
}

void LinuxPacketReader::stopListening() {

}

Net::Packet LinuxPacketReader::nextPacket() const {
    int result;
    ssize_t data_size;
    socklen_t saddr_size;
    struct sockaddr saddr;
    fd_set readfs = readset;

    result = select(FD_SETSIZE, &readfs, NULL, NULL, NULL);
    (void)result;
    std::cout << "set socket ok" << std::endl;;
    for (int i = 0; i < 3; i++) {
        int socket = sockets[i];
        if (FD_ISSET(socket, &readfs)) {
            std::cout << "read fd ready : " << socket << std::endl;
            saddr_size = sizeof saddr;
            //Receive a packet
            data_size = recvfrom(socket, _buffer.get(), 65536, 0, &saddr, &saddr_size);
            std::cout << "read data : [" << data_size << std::endl;
            if (data_size < 0) {
                throw lastSystemError();
            }
            Net::Packet packet = Net::Packet(data_t(_buffer.get(), data_size));
	    return packet;
        }
    }
    return Net::Packet(NULL);
}

}
