/*
 * Copyright 2016 <copyright holder> <email>
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

#ifndef LINUXPACKETREADER_H
#define LINUXPACKETREADER_H

#include <string.h>
#include <iostream>
#include <unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
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
