#ifndef PCAPPACKET_H
#define PCAPPACKET_H

#include <memory>
#include <string>
#include <stdint.h>

#include <iostream>

#include "Utils.h"

namespace Net {

class PcapPacket {

typedef struct pcaprec_hdr_s {
    uint32_t ts_sec;         /* timestamp seconds */
    uint32_t ts_usec;        /* timestamp microseconds */
    uint32_t incl_len;       /* number of octets of packet saved in file */
    uint32_t orig_len;       /* actual length of packet */
} pcaprec_hdr_t;

public:
    template<typename ITERATOR>
    PcapPacket(ITERATOR& start, ITERATOR const& end, bool swap, size_t maxLength);
    ~PcapPacket() = default;

    //TODO timestamp to std::date ?

    data_t& packet() { return _packet; };
    data_t const& packet() const { return _packet; };

    bool isIncomplete() const { return _header.incl_len != _header.orig_len; };

    data_t getRawData() const;
private:
    bool _needSwap;
    pcaprec_hdr_t _header;
    data_t _packet;
};

template<typename ITERATOR>
PcapPacket::PcapPacket(ITERATOR& start, ITERATOR const& end, bool swap, size_t maxLength) {
    if (static_cast<size_t>(std::distance(start, end)) < sizeof(_header)) {
        throw std::runtime_error("Error parsing packet header.");
    }
    auto tmp = std::next(start, sizeof(_header));
    std::copy(start, tmp, reinterpret_cast<char*>(&_header));
    start = tmp;
    if (swap) {
       _header.ts_sec = switchEndianness(_header.ts_sec);
       _header.ts_usec = switchEndianness(_header.ts_usec);
       _header.incl_len = switchEndianness(_header.incl_len);
       _header.orig_len = switchEndianness(_header.orig_len);
    }
    if (_header.incl_len > maxLength) {
       std::cerr << "Warning length of packet is larger than max length, continuing anyways." << std::endl;
    }
    if (static_cast<size_t>(std::distance(start, end)) < _header.incl_len) {
        throw std::runtime_error("Error parsing packet data.");
    }
    tmp = std::next(start, _header.incl_len);
    std::copy(start, tmp, std::back_inserter(_packet));
    start = tmp;
}

}

#endif // PCAPPACKET_H
