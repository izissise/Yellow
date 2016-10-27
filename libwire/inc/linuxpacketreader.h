#ifndef LINUXPACKETREADER_H
#define LINUXPACKETREADER_H

#include <string.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "PacketReader.hpp"

namespace Net {
	class LinuxPacketReader: public PacketReader {
	private:
		unsigned char *buffer;
		int sockets[3];
		fd_set readset;
	public:
		void startListening(std::string const interfaceName);
        void stopListening();
        void nextPacket();
	};
}

#endif // LINUXPACKETREADER_H
