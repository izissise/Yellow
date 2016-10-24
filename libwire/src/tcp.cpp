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

#include <netinet/in.h>
#include "tcp.h"

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