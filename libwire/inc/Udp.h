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
    constexpr static size_t HeaderSize = sizeof(udphdr);
public:
    //! @throw WrongSize
    Udp(data_slice_t const& slice);
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

protected:
    data_slice_t getHeaderBasePtr() const override {
        return data_slice_t(reinterpret_cast<uint8_t*>(_udpHeader), HeaderSize);
    }

private:
    udphdr* _udpHeader;
};

}

#endif // UDP_H
