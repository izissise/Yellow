#include "packet.h"

#include "IpHeader.h"
#include "udp.h"
#include "tcp.h"

namespace Net {

Packet::Packet(data_t const& buffer)
: ipHeader(Net::IpHeader(buffer)) {
    processPacket(buffer);
}

void Packet::processPacket(data_t const& buffer) {
    unsigned int ip_header_len = ipHeader.ihl() * 4;
    ipHeader.debugDisplay();

    data_t rawData((buffer.data())[ip_header_len], buffer.size() - ip_header_len);
    switch (ipHeader.protocol())
    {
    case 1:  //ICMP Protocol
        //PrintIcmpPacket(Buffer,Size);
        break;

    case 6: {
        //TCP Protocol
        Net::Tcp tcp(rawData);
        tcp.debugDisplay();
        //print_tcp_packet(buffer , size);
        break;
    }
    case 17: {
        //UDP Protocol
        Net::Udp udp(rawData);
        udp.debugDisplay();
        //print_udp_packet(buffer , size);
        break;
    }
    default:
        break;
    }
}

}
