#include <iostream>

#include <string.h>
#include <arpa/inet.h>

#include "IpHeader.h"

namespace Net {
	IpHeader::IpHeader(unsigned char *buffer) {
		ip_header = (struct iphdr*)buffer;
		
		struct sockaddr_in source,dest;
		memset(&source, 0, sizeof(source));
		source.sin_addr.s_addr = ip_header->saddr;

		memset(&dest, 0, sizeof(dest));
		dest.sin_addr.s_addr = ip_header->daddr;
		
		saddr = std::string(inet_ntoa(source.sin_addr));
		daddr = std::string(inet_ntoa(dest.sin_addr));
    }

	IpHeader::~IpHeader() {}
	bool IpHeader::operator==(const IpHeader& other) const { return true; }

	void IpHeader::debugDisplay() {
		std::cout << "   |-IP Version          " << (unsigned int)ip_header->version << std::endl;
		std::cout << "   |-IP Header Length  : " << (unsigned int)ip_header->ihl << std::endl;
		std::cout << "   |-Type Of Service   : " << (unsigned int)ip_header->tos << std::endl;
		std::cout << "   |-IP Total Length   : " << (unsigned int)ip_header->tot_len << std::endl;
		std::cout << "   |-Identification    : " << (unsigned int)ip_header->id << std::endl;
		std::cout << "   |-TTL               : " << (unsigned int)ip_header->ttl << std::endl;
		std::cout << "   |-Protocol          : " << (unsigned int)ip_header->protocol << std::endl;
		std::cout << "   |-Checksum          : " << (unsigned int)ip_header->check << std::endl;
		std::cout << "   |-Source IP         : " << saddr << std::endl;
		std::cout << "   |-Destination IP    : " << daddr << std::endl;
	}
}
