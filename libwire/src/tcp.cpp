#include "tcp.h"

#include <netinet/in.h>

namespace Net {
	Tcp::Tcp(unsigned char *buffer) {
		tcp_struct = (struct tcphdr*)(buffer);
	}

	void Tcp::debugDisplay() {
		std::cout << "TCP Header" << std::endl;
		std::cout << "   |-Source Port        : " << ntohs(tcp_struct->th_sport) << std::endl;
		std::cout << "   |-Destination Port   : " << ntohs(tcp_struct->th_dport) << std::endl;
		std::cout << "   |-Sequence number    : " << ntohs(tcp_struct->th_seq) << std::endl;
		std::cout << "   |-Acknowledge Number : " << ntohs(tcp_struct->th_ack) << std::endl;
		std::cout << "   |-Header Length      : " << ntohs(tcp_struct->th_off) << std::endl;
		std::cout << "   |-Flags              : " << ntohs(tcp_struct->th_flags) << std::endl;
	}
}
