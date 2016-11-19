#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>
#include <asm/types.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

#include "NetAddr.h"

#include "ATransport.h"

namespace Net {

class Icmp : public ATransport {
    constexpr static size_t HeaderSize = sizeof(icmphdr);
public:
    //! @throw WrongSize
    Icmp(data_slice_t const& slice);
    virtual ~Icmp() = default;

    Net::Transport type() const override { return Net::Transport::ICMP; }

    uint8_t icmpType() const { return _icmpHeader->type; }
    uint8_t code() const { return _icmpHeader->code; }
    uint16_t sum() const { return ntohs(_icmpHeader->checksum); }
    uint16_t id() const { return ntohs(_icmpHeader->un.echo.id); }
    uint16_t seq() const { return ntohs(_icmpHeader->un.echo.sequence); }

    Net::NetAddr gateway() const { return NetAddr(_icmpHeader->un.gateway); };

protected:
    data_slice_t getHeaderBasePtr() const override {
        return data_slice_t(reinterpret_cast<uint8_t*>(_icmpHeader), HeaderSize);
    }

private:
    icmphdr* _icmpHeader;
};
}

#endif // ICMP_H
