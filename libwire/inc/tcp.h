#ifndef TCP_H
#define TCP_H

#include <iostream>
#include <stdint.h>
#include <asm/types.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>

namespace Net {
	
	struct tcphdr {
		u_short	th_sport;		/* source port */
		u_short	th_dport;		/* destination port */
		u_long	th_seq;			/* sequence number */
		u_long	th_ack;			/* acknowledgement number */
	#if BYTE_ORDER == LITTLE_ENDIAN 
		u_char	th_x2:4,		/* (unused) */
		th_off:4;		/* data offset */
	#endif
	#if BYTE_ORDER == BIG_ENDIAN 
		u_char	th_off:4,		/* data offset */
		th_x2:4;		/* (unused) */
	#endif
		u_char	th_flags;
		u_short	th_win;			/* window */
		u_short	th_sum;			/* checksum */
		u_short	th_urp;			/* urgent pointer */
	};
	
	class Tcp {
	private:
		struct tcphdr *tcp_struct;
	public:
		Tcp(unsigned char *buffer);
		void debugDisplay();
		u_short th_sport() const { return tcp_struct->th_sport; }
		u_short th_dport() const { return tcp_struct->th_dport; }
		u_long th_seq() const { return tcp_struct->th_seq; }
		u_long th_ack() const { return tcp_struct->th_ack; }
		u_char th_off() const { return tcp_struct->th_off; }
		u_char th_flags() const { return tcp_struct->th_flags; }
		u_short th_win() const { return tcp_struct->th_win; }
		u_short th_sum() const { return tcp_struct->th_sum; }
		u_short th_urp() const { return tcp_struct->th_urp; }
	};
}

#endif // TCP_H