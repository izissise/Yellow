#include "Packet.h"

#include "IpHeader.h"
#include "Udp.h"
#include "Tcp.h"

namespace Net {

Packet::Packet(data_t const& buffer)
: Net::PcapPacket(buffer) {
    processPacket(_packet);
}

void Packet::processPacket(data_t const& buffer) {
    _ipHeader = ipHeaderPlacementNew(_ipHeaderStore, Version::V4, const_cast<uint8_t*>(buffer.data()), buffer.size());


//     unsigned int ip_header_len = _ipHeader.ihl() * 4;
//     _ipHeader.debugDisplay();
//
//     data_t rawData(&((buffer.data())[ip_header_len]), buffer.size() - ip_header_len);
//     switch (_ipHeader.protocol())
//     {
//     case 1:  //ICMP Protocol
//         //PrintIcmpPacket(Buffer,Size);
//         break;
//
//     case 6: {
//         //TCP Protocol
//         Net::Tcp tcp(rawData);
// 	_type = TCP;
//         tcp.debugDisplay();
//         //print_tcp_packet(buffer , size);
//         break;
//     }
//     case 17: {
//         //UDP Protocol
//         Net::Udp udp(rawData);
// 	_type = UDP;
//         udp.debugDisplay();
//         //print_udp_packet(buffer , size);
//         break;
//     }
//     default:
//         break;
//     }
}

}
