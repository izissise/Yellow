#include "PcapPacket.h"

namespace Net {

PcapPacket::PcapPacket(data_t const& data, std::chrono::time_point<std::chrono::high_resolution_clock> const& date)
: _packet(data) {
    _header.incl_len = static_cast<uint32_t>(data.size());
    _header.orig_len = _header.incl_len;
    _header.ts_sec =  static_cast<uint32_t>(std::chrono::system_clock::to_time_t(date));
    _header.ts_usec = static_cast<uint32_t>((date - std::chrono::time_point_cast<std::chrono::seconds>(date)).count());
}

std::chrono::time_point<std::chrono::high_resolution_clock> PcapPacket::date() const {
    auto ret = std::chrono::system_clock::from_time_t(std::time_t(_header.ts_sec));
    ret += std::chrono::microseconds(_header.ts_usec);
    return ret;
}

data_t PcapPacket::getRawData() const {
    return data_t(reinterpret_cast<const char*>(&_header), sizeof(_header)) + _packet;
}

}
