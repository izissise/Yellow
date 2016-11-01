#ifndef PACKET_H
#define PACKET_H

#include "IpHeader.h"

#include "Utils.h"

namespace Net {

  enum PacketType {
    UDP,
    TCP,
    ICMP
  };

class Packet
{
public:
    Packet(data_t const& buffer);

private:
    Net::IpHeader ipHeader;
    PacketType type;
    void processPacket(data_t const& buffer);
};

}

#endif // PACKET_H
