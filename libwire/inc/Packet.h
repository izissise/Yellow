#ifndef PACKET_H
#define PACKET_H

#include <algorithm>

#include "IpHeader.h"
#include "PcapPacket.h"
#include "NetUtils.h"

namespace Net {

class Packet : public PcapPacket {
public:
    Packet(data_t const& buffer);
    virtual ~Packet() = default;

private:
    void processPacket(data_t const& buffer);

private:
    //Ethernet Header
    Net::IIpHeader* _ipHeader = nullptr; // Will be in ethernet header

// Class Objects storage
private:
    uint8_t _ipHeaderStore[std::max(sizeof(IpHeaderV4), sizeof(IpHeaderV6))];
};

}

#endif // PACKET_H
