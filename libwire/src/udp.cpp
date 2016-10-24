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
#include "udp.h"

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