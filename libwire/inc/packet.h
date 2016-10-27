#ifndef PACKET_H
#define PACKET_H

#include "IpHeader.h"

namespace Net {
	class Packet
	{
	public:
		Net::IpHeader ipHeader;
		Packet(unsigned char *buffer);
	private:
		void processPacket(unsigned char *buffer);
	};
}

#endif // PACKET_H
