#include "PcapPacket.h"

namespace Net {

PcapPacket::PcapPacket(data_t const& data, std::chrono::time_point<std::chrono::high_resolution_clock> const& date)
: _packet(data) {
    _header.incl_len = static_cast<uint32_t>(_packet.size());
    _header.orig_len = _header.incl_len;

    setDate(date);
}

void PcapPacket::setPacket(const data_t& data){
    _packet = data;

    _header.incl_len = static_cast<uint32_t>(data.size());
    _header.orig_len = _header.incl_len;
}

void PcapPacket::setDate(std::chrono::time_point<std::chrono::high_resolution_clock> const& date) {
    auto durationFromEpoch = std::chrono::duration_cast<std::chrono::microseconds>(date.time_since_epoch());
    auto dSec = std::chrono::duration_cast<std::chrono::seconds>(durationFromEpoch);
    auto dMs = std::chrono::duration_cast<std::chrono::microseconds>(durationFromEpoch - dSec);

    _header.ts_sec =  static_cast<uint32_t>(dSec.count());
    _header.ts_usec = static_cast<uint32_t>(dMs.count());
}

std::chrono::time_point<std::chrono::high_resolution_clock> PcapPacket::date() const {
    auto ret = std::chrono::high_resolution_clock::from_time_t(std::time_t(_header.ts_sec));
    ret += std::chrono::microseconds(_header.ts_usec);
    return ret;
}

data_t PcapPacket::getRawHeaderAndData() const {
    data_t ret(reinterpret_cast<const uint8_t*>(&_header), &((reinterpret_cast<const uint8_t*>(&_header))[sizeof(_header)]));
    ret.insert(std::end(ret), std::begin(_packet), std::end(_packet));
    return ret;
}

}
