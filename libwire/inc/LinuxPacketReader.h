#ifndef LINUXPACKETREADER_H
#define LINUXPACKETREADER_H

#include <iostream>
#include <memory>
#include <array>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "Socket.h"
#include "Tcp.h"
#include "Udp.h"
#include "Icmp.h"

#include "PacketReader.hpp"
#include "Packet.h"

#include "Utils.h"

namespace Net {

class LinuxPacketReader: public PacketReader {
public:
    LinuxPacketReader();
    virtual ~LinuxPacketReader() = default;

    //! @throw std::system_error
    void startListening(Net::InterfaceInfo const& interface) override;

    //! @throw std::system_error
    void stopListening() override;

    Net::Packet nextPacket() const override;

private:
    std::array<std::unique_ptr<ISocket>, 3> _sockets;
    std::unique_ptr<uint8_t[]> _buffer;
    fd_set readset;
};

}

#endif // LINUXPACKETREADER_H
