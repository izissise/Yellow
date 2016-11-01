#include "PcapPacket.h"

namespace Net {

PcapPacket::PcapPacket(data_t const& data, std::chrono::time_point<std::chrono::high_resolution_clock> const& date)
: _packet(data) {
    auto durationFromEpoch = std::chrono::duration_cast<std::chrono::microseconds>(date.time_since_epoch());
    auto dSec = std::chrono::duration_cast<std::chrono::seconds>(durationFromEpoch);
    auto dMs = std::chrono::duration_cast<std::chrono::microseconds>(durationFromEpoch - dSec);

    _header.incl_len = static_cast<uint32_t>(data.size());
    _header.orig_len = _header.incl_len;
    _header.ts_sec =  static_cast<uint32_t>(dSec.count());
    _header.ts_usec = static_cast<uint32_t>(dMs.count());
}

std::chrono::time_point<std::chrono::high_resolution_clock> PcapPacket::date() const {
    auto ret = std::chrono::high_resolution_clock::from_time_t(std::time_t(_header.ts_sec));
    ret += std::chrono::microseconds(_header.ts_usec);
    return ret;
}

data_t PcapPacket::getRawData() const {
    return data_t(reinterpret_cast<const uint8_t*>(&_header), sizeof(_header)) + _packet;
}

}
