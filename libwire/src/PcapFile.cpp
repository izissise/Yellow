#include "PcapFile.h"

namespace Net {

PcapFile::PcapFile()
: _needSwap(false) {
    _header.magic_number = magic_number;
    _header.version_major = 2;
    _header.version_minor = 4;
    _header.thiszone = 0;
    _header.sigfigs = 0;
    _header.snaplen = 262144;
    _header.network = 1;
}

std::tuple<uint16_t, uint16_t> PcapFile::versionNumber() const {
    return std::make_tuple(_header.version_major, _header.version_minor);
}

void PcapFile::versionNumber(uint16_t major, uint16_t minor) {
    _header.version_major = major;
    _header.version_minor = minor;
}

void PcapFile::addPacket(Net::PcapPacket packet) {
  if (packet.size() > _header.snaplen) {
      _header.snaplen = packet.size();
  }
  _packets.push_back(packet);
}

}
