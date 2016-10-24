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

#ifndef IPHEADER_H
#define IPHEADER_H

#include <iostream>
#include <stdint.h>
#include <asm/types.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>

namespace Net {

	struct iphdr {
	#if defined(__LITTLE_ENDIAN_BITFIELD)
		__u8	ihl:4,
		version:4;
	#	elif defined (__BIG_ENDIAN_BITFIELD)
		__u8	version:4,
		ihl:4;
	#else
	#error	"Please fix <asm/byteorder.h>"
	#endif
	__u8	tos;
	__be16	tot_len;
	__be16	id;
	__be16	frag_off;
	__u8	ttl;
	__u8	protocol;
	__sum16	check;
	__be32	saddr;
	__be32	daddr;
	/*The options start here. */
	};
	
	class IpHeader
	{
	private:
		struct iphdr *ip_header;
	public:
		explicit IpHeader(unsigned char *buffer);
		~IpHeader();
		bool operator==(const IpHeader& other) const;
		void debugDisplay();
		int8_t ihl() const { return ip_header->ihl; }
		int8_t tos() const { return ip_header->tos; }
		int16_t tot_len() const { return ip_header->tot_len; }
		int16_t id() const { return ip_header->id; }
		int16_t frag_off() const { return ip_header->frag_off; }
		int8_t ttl() const { return ip_header->ttl; }
		int8_t protocol() const { return ip_header->protocol; }
		int16_t check() const { return ip_header->check; }
		std::string saddr;
		std::string daddr;
	};
}

#endif // IPHEADER_H
