#include "udp.h"

#include <netinet/in.h>

namespace Net {
	Udp::Udp(unsigned char *buffer) {
		udp_struct = (struct udphdr*)(buffer);
	}
	void Udp::debugDisplay() {
		std::cout << "UDP Header" << std::endl;
		std::cout << "   |-Source Port      : " << ntohs(udp_struct->source) << std::endl;
		std::cout << "   |-Destination Port : " << ntohs(udp_struct->dest) << std::endl;
		std::cout << "   |-UDP Length       : " << ntohs(udp_struct->len) << std::endl;
		std::cout << "   |-UDP Checksum     : " << ntohs(udp_struct->check) << std::endl;
	}
}
