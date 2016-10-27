#include "packet.h"

#include "IpHeader.h"
#include "udp.h"
#include "tcp.h"

namespace Net {
	Packet::Packet(unsigned char* buffer) : ipHeader(Net::IpHeader(buffer)) {
		processPacket(buffer);
	}
	
	void Packet::processPacket(unsigned char *buffer) {
		unsigned int ip_header_len = ipHeader.ihl() * 4;
		ipHeader.debugDisplay();
		switch ((int)ipHeader.protocol())
		{
		case 1:  //ICMP Protocol
            //PrintIcmpPacket(Buffer,Size);
            break;

        case 6: {
			//TCP Protocol
			Net::Tcp tcp = Net::Tcp(buffer + ip_header_len);
			tcp.debugDisplay();
			//print_tcp_packet(buffer , size);
            break;
		}         
        case 17: {
			//UDP Protocol
			Net::Udp udp = Net::Udp(buffer + ip_header_len);
			udp.debugDisplay();
            //print_udp_packet(buffer , size);
            break;
		}
        default:
            break;
		}		
	}
}
