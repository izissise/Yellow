#ifndef PACKET_H
#define PACKET_H

#include "IpHeader.h"

#include "Utils.h"

namespace Net {

class Packet
{
public:
    Net::IpHeader ipHeader;
    Packet(data_t const& buffer);

private:
    void processPacket(data_t const& buffer);
};

}

#endif // PACKET_H
