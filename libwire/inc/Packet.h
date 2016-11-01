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
    Net::IpHeader ipHeader;
    PacketType type;
    Packet(data_t const& buffer);

private:
    void processPacket(data_t const& buffer);
};

}

#endif // PACKET_H
