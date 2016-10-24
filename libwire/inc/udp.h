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

#ifndef UDP_H
#define UDP_H

#include <iostream>
#include <stdint.h>
#include <asm/types.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>

namespace Net {
	
	struct udphdr {
		u_int16_t	source;
		u_int16_t	dest;
		u_int16_t	len;
		u_int16_t	check;
	};

	class Udp {
	private:
		struct udphdr *udp_struct;

	public:
		Udp(unsigned char *buffer);
		void debugDisplay();
		int16_t source() const { return udp_struct->source; }
		int16_t dest() const { return udp_struct->dest; }
		int16_t len() const { return udp_struct->len; }
		int16_t check() const { return udp_struct->check; }
	};
}

#endif // UDP_H
