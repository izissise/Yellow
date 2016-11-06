#ifndef TCP_H
#define TCP_H

#include <iostream>
#include <stdint.h>
#include <netinet/in.h>
#include <asm/types.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>

#include "Utils.h"

namespace Net {


class Tcp {

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

public:
    Tcp(data_t const& buffer);
    ~Tcp() = default;

    void debugDisplay();
    u_short th_sport() const { return _tcpStruct.th_sport; }
    u_short th_dport() const { return _tcpStruct.th_dport; }
    u_long th_seq() const { return _tcpStruct.th_seq; }
    u_long th_ack() const { return _tcpStruct.th_ack; }
    u_char th_off() const { return _tcpStruct.th_off; }
    u_char th_flags() const { return _tcpStruct.th_flags; }
    u_short th_win() const { return _tcpStruct.th_win; }
    u_short th_sum() const { return _tcpStruct.th_sum; }
    u_short th_urp() const { return _tcpStruct.th_urp; }

private:
    struct tcphdr _tcpStruct;
};

}

#endif // TCP_H
