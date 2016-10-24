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

#include "linuxpacketreader.h"
#include "packet.h"

namespace Net {
	
	void LinuxPacketReader::startListening(const std::string interfaceName) {
		buffer = (unsigned char *)malloc(65536);
		sockets[0] = socket(AF_INET , SOCK_RAW , IPPROTO_TCP);
		sockets[1] = socket(AF_INET , SOCK_RAW , IPPROTO_UDP);
		sockets[2] = socket(AF_INET , SOCK_RAW , IPPROTO_ICMP);
		FD_ZERO(&readset);
		
		int fd_max = 0;
		for (int i = 0; i < 3; i++) {
			int socket = sockets[i];
			if (socket < 0) {
				printf("Socket Error\n");
				return ;
			}
			if (socket > fd_max) {
				fd_max = socket;
			}
			printf("add socket fd : %d\n", socket);
			FD_SET(socket, &readset);
		}
	}
	
	void LinuxPacketReader::stopListening() {

	}
	
	void LinuxPacketReader::nextPacket() {
		int result;
		int data_size;
		socklen_t saddr_size; 
		struct sockaddr saddr;
		fd_set readfs = readset;
		
		result = select(FD_SETSIZE, &readfs, NULL, NULL, NULL);
		printf("set socket ok\n");
		for (int i = 0; i < 3; i++) {
			int socket = sockets[i];
			if (FD_ISSET(socket, &readfs)) {
				printf("read fd ready : %d\n", socket);
				saddr_size = sizeof saddr;
				//Receive a packet
				data_size = recvfrom(socket , buffer , 65536 , 0 , &saddr , &saddr_size);
				printf("read data : [%d]\n", data_size);
				if (data_size < 0) {
					printf("Recvfrom error , failed to get packets\n");
					return ;
				}
				Net::Packet packet = Net::Packet(buffer);
			}	
		}
	}
}