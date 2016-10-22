#include "PcapPacket.h"

namespace Net {

PcapPacket::PcapPacket(data_t const& data)
: _packet(data) {
    _header.incl_len = static_cast<uint32_t>(data.size());
    _header.orig_len = _header.incl_len;
    _header.ts_sec = 0;
    _header.ts_usec = 0;
}

data_t PcapPacket::getRawData() const {
    return data_t(reinterpret_cast<const char*>(&_header), sizeof(_header)) + _packet;
}

}
