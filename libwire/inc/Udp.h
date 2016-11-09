#ifndef UDP_H
#define UDP_H

#include <iostream>
#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <asm/types.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>

#include "ATransport.h"

namespace Net {

class Udp : public ATransport {
public:
    Udp(uint8_t* buffer, size_t buffsize);
    virtual ~Udp() = default;

    Net::Transport type() const override { return Net::Transport::UDP; }

    uint16_t srcPort() const { return ntohs(_udpHeader->uh_sport); }
    void     srcPort(uint16_t sport) { _udpHeader->uh_sport = htons(sport); }

    uint16_t dstPort() const { return ntohs(_udpHeader->uh_dport); }
    void     dstPort(uint16_t dport) { _udpHeader->uh_dport = htons(dport); }

    uint16_t dataSize() const { return  ntohs(_udpHeader->uh_ulen); };
    void     dataSize(uint16_t dSize) { _udpHeader->uh_ulen = htons(dSize); }

    uint16_t checksum() const { return  ntohs(_udpHeader->uh_sum); };
    void     checksum(uint16_t sum) { _udpHeader->uh_sum = htons(sum); }

private:
    udphdr* _udpHeader;
};

}

#endif // UDP_H
