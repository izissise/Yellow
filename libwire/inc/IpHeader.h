#ifndef IPHEADER_H
#define IPHEADER_H

#include <string>

#include <stdint.h>
#include <asm/types.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>

#include "NetUtils.h"
#include "Utils.h"

namespace Net {

    typedef struct iphdr {
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
    } iphdr_t;
	
	class IpHeader
	{
	public:
		explicit IpHeader(data_t const& buffer);
		~IpHeader();
		bool operator==(const IpHeader& other) const;
		void debugDisplay();
		int8_t ihl() const { return _ipHeader.ihl; }
		int8_t tos() const { return _ipHeader.tos; }
		int16_t tot_len() const { return _ipHeader.tot_len; }
		int16_t id() const { return _ipHeader.id; }
		int16_t frag_off() const { return _ipHeader.frag_off; }
		int8_t ttl() const { return _ipHeader.ttl; }
		int8_t protocol() const { return _ipHeader.protocol; }
		int16_t check() const { return _ipHeader.check; }

		Net::NetAddr srcAddr() const { return _saddr; };
		Net::NetAddr dstAddr() const { return _daddr; };

	private:
        iphdr_t _ipHeader;
		NetAddr _saddr;
        NetAddr _daddr;
	};
}

#endif // IPHEADER_H
