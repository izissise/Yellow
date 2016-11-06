#include "EthernetFrame.h"

namespace Net {

EthernetFrame::EthernetFrame(uint8_t* buffer, size_t buffsize) {
    constexpr size_t headerSize = sizeof(ether_header);
    if (buffsize < headerSize) {
        throw WrongSize("Error parsing ethernet header.", headerSize - buffsize, headerSize);
    }
    _header = reinterpret_cast<ether_header*>(buffer);

    buffsize = buffsize - headerSize;
    buffer = &(buffer[headerSize]);

    switch(type()) {
        case ETHERTYPE_IP:
            _ipHeader = ipHeaderPlacementNew(_ipHeaderStore, Version::V4, buffer, buffsize);
        break;
        case ETHERTYPE_IPV6:
            _ipHeader = ipHeaderPlacementNew(_ipHeaderStore, Version::V6, buffer, buffsize);
        break;
        default:
            _ipHeader = ipHeaderPlacementNew(_ipHeaderStore, Version::V4, buffer, buffsize);
        break;
    }
}

EthernetFrame::~EthernetFrame() {
    if (_ipHeader != nullptr) {
        _ipHeader->~IIpHeader();
    }
}

std::string EthernetFrame::srcAddr() const {
    auto addr = switchEndianness(_header->ether_shost);
    return stringToHex(std::string(std::begin(addr), std::end(addr)), ":", true);
}

std::string EthernetFrame::dstAddr() const {
    auto addr = switchEndianness(_header->ether_dhost);
    return stringToHex(std::string(std::begin(addr), std::end(addr)), ":", true);
}

}
