#include "PcapFile.h"

namespace Net {

template<>
PcapFile<>::PcapFile() {
    _header.magic_number = magic_number;
}

template<>
std::tuple<uint16_t, uint16_t> PcapFile<>::versionNumber() const {
    return std::make_tuple(_header.version_major, _header.version_minor);
};

template<>
void PcapFile<>::versionNumber(uint16_t major, uint16_t minor) {
  _header.version_major = major;
  _header.version_minor = minor;
}

}
