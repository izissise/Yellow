#ifndef PACKET_H
#define PACKET_H

#include <algorithm>

#include "IpHeader.h"
#include "EthernetFrame.h"
#include "PcapPacket.h"
#include "NetUtils.h"

namespace Net {

class Packet : public PcapPacket {
public:
    Packet(data_t const& buffer);
    virtual ~Packet() = default;

    Packet(Packet const& o);
    Packet& operator=(Packet const& o) = delete;

    EthernetFrame const& ethernetFrame() const { return _ethernetHeader; };
    EthernetFrame& ethernetFrame() { return _ethernetHeader; };

private:
    EthernetFrame _ethernetHeader;
};

}

#endif // PACKET_H
