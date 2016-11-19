#ifndef TCP_H
#define TCP_H

#include <stdint.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <asm/types.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>

#include "ATransport.h"

namespace Net {

class Tcp : public ATransport {
    constexpr static size_t HeaderSize = sizeof(tcphdr);
public:
    //! @throw WrongSize
    Tcp(data_slice_t const& slice);
    virtual ~Tcp() = default;

    Net::Transport type() const override { return Net::Transport::TCP; }

    uint16_t srcPort() const { return ntohs(_tcpHeader->th_sport); }
    void     srcPort(uint16_t sport) { _tcpHeader->th_sport = htons(sport); }

    uint16_t dstPort() const { return ntohs(_tcpHeader->th_dport); }
    void     dstPort(uint16_t dport) { _tcpHeader->th_dport = htons(dport); }

    uint32_t seq() const { return ntohl(_tcpHeader->th_seq); }
    uint32_t ack() const { return ntohl(_tcpHeader->th_ack); }
    uint8_t offset() const { return _tcpHeader->th_off; }
    uint8_t flags() const { return _tcpHeader->th_flags; }
    uint16_t window() const { return ntohs(_tcpHeader->th_win); }
    uint16_t sum() const { return ntohs(_tcpHeader->th_sum); }
    uint16_t urg() const { return ntohs(_tcpHeader->th_urp); }

protected:
    data_slice_t getHeaderBasePtr() const override {
        return data_slice_t(reinterpret_cast<uint8_t*>(_tcpHeader), HeaderSize);
    }

private:
    tcphdr* _tcpHeader;
};

}

#endif // TCP_H
