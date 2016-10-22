#ifndef PCAPPACKET_H
#define PCAPPACKET_H

#include <memory>
#include <string>
#include <chrono>
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
    //! @throw WrongSize
    //! @throw std::system_error
    template<class STREAM>
    PcapPacket(STREAM& stream, bool swap, size_t maxLength);

    explicit PcapPacket(data_t const& data, std::chrono::time_point<std::chrono::high_resolution_clock> const& date = std::chrono::high_resolution_clock::now());

    ~PcapPacket() = default;

    std::chrono::time_point<std::chrono::high_resolution_clock> date() const;

    data_t const& packet() const { return _packet; };

    bool isIncomplete() const { return _header.incl_len != _header.orig_len; };
    uint32_t size() const { return _header.incl_len; };

    data_t getRawData() const;
protected:
    pcaprec_hdr_t _header;
    data_t _packet;
};

template<class STREAM>
PcapPacket::PcapPacket(STREAM& stream, bool swap, size_t maxLength) {
   size_t n = 0;

   if ((n = stream.read(reinterpret_cast<char*>(&_header), sizeof(_header)).gcount()) != sizeof(_header)) {
        throw WrongSize("Error parsing packet header.", sizeof(_header) - n, sizeof(_header));
    }
    if (swap) {
       _header.ts_sec = switchEndianness(_header.ts_sec);
       _header.ts_usec = switchEndianness(_header.ts_usec);
       _header.incl_len = switchEndianness(_header.incl_len);
       _header.orig_len = switchEndianness(_header.orig_len);
    }
    if (_header.incl_len > maxLength) {
       std::cerr << "Warning length of packet is larger than max length, continuing anyways." << std::endl;
    }
//     auto it = std::istreambuf_iterator<char>(stream);
//     if ((n = std::distance(it, std::istreambuf_iterator<char>())) < _header.incl_len) {
//         WrongSize("Error parsing packet data.", _header.incl_len - n, _header.incl_len);
//     }
//     _packet.assign(it, std::next(it, _header.incl_len));

    auto buff = std::make_unique<char[]>(_header.incl_len);
    if ((n = stream.read(buff.get(), _header.incl_len).gcount()) < _header.incl_len) {
        WrongSize("Error parsing packet data.", _header.incl_len - n, _header.incl_len);
    }
    _packet = std::move(data_t(buff.get(), _header.incl_len));
}

}

#endif // PCAPPACKET_H
