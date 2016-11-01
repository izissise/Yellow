#ifndef LINUXPACKETREADER_H
#define LINUXPACKETREADER_H

#include <iostream>
#include <memory>
#include <array>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "PacketReader.hpp"
#include "Packet.h"

namespace Net {

class LinuxPacketReader: public PacketReader {
public:
    LinuxPacketReader();
    virtual ~LinuxPacketReader() = default;

    void startListening(Net::InterfaceInfo const& interface) override;
    void stopListening() override;
    Net::Packet nextPacket() const override;

private:
    int createRAWSocket(Net::InterfaceInfo const& interface, int protocol) const;

private:
    std::unique_ptr<uint8_t[]> _buffer;
    std::array<int, 3> sockets;
    fd_set readset;
};

}

#endif // LINUXPACKETREADER_H
