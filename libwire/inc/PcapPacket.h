#ifndef PCAPPACKET_H
#define PCAPPACKET_H

#include <memory>
#include <string>
#include <stdint.h>

#include "Utils.h"

namespace Net {

template<class DATA = data_t>
class PcapPacket {

typedef struct pcaprec_hdr_s {
    uint32_t ts_sec;         /* timestamp seconds */
    uint32_t ts_usec;        /* timestamp microseconds */
    uint32_t incl_len;       /* number of octets of packet saved in file */
    uint32_t orig_len;       /* actual length of packet */
} pcaprec_hdr_t;

public:
    PcapPacket(DATA const& data, typename DATA::const_iterator& pos, size_t maxLength);
    ~PcapPacket() = default;

    // timestamp to std::date ?

    data_t& packet() { return _packet; };
    data_t const& packet() const { return _packet; };

    bool isIncomplete() const { return _header.incl_len != _header.orig_len; };
private:
    bool _needSwap;
    pcaprec_hdr_t _header;
    data_t _packet;
};

template<class DATA>
PcapPacket<DATA>::PcapPacket(DATA const& data, typename DATA::const_iterator& pos, size_t maxLength) {

}

}

#endif // PCAPPACKET_H
