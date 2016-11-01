#include "LinuxPacketReader.h"

#include "Packet.h"

namespace Net {

LinuxPacketReader::LinuxPacketReader()
: _sockets({ std::make_unique<Socket<Tcp>>(),
             std::make_unique<Socket<Udp>>(),
             std::make_unique<Socket<Icmp>>() }),
 _buffer(std::make_unique<uint8_t[]>(65536)) {
}

void LinuxPacketReader::startListening(Net::InterfaceInfo const& interface) {
    for (auto& sock : _sockets) {
       sock->startListening(interface);
    }
    FD_ZERO(&readset);

    int fd_max = 0;
    for (int i = 0; i < 3; i++) {
        int socket = _sockets[i]->getFd();
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
    for (auto& sock : _sockets) {
        sock->stopListening();
    }
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
        int socket = _sockets[i]->getFd();
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
