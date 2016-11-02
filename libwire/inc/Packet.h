#ifndef PACKET_H
#define PACKET_H

#include "IpHeader.h"

#include "NetUtils.h"

namespace Net {

class Packet
{
public:
    Packet(data_t const& buffer);

private:
    void processPacket(data_t const& buffer);

private:
    Net::IpHeader _ipHeader;
    Protocol _type;
};

}

#endif // PACKET_H
