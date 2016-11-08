#include "EthernetFrame.h"

#include <array>
#include <cstring>

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
            _ipHeader.reset(networkLayerCreate(Version::V4, buffer, buffsize));
        break;
        case ETHERTYPE_IPV6:
            _ipHeader.reset(networkLayerCreate(Version::V6, buffer, buffsize));
        break;
        default:
            _ipHeader.reset(networkLayerCreate(Version::V4, buffer, buffsize));
        break;
    }
}

std::string EthernetFrame::srcAddr() const {
    std::array<char, ETH_ALEN> addr;
    std::memcpy(addr.data(), _header->ether_shost, addr.size());
    return stringToHex(std::string(std::begin(addr), std::end(addr)), ":", true);
}

std::string EthernetFrame::dstAddr() const {
    std::array<char, ETH_ALEN> addr;
    std::memcpy(addr.data(), _header->ether_dhost, addr.size());
    return stringToHex(std::string(std::begin(addr), std::end(addr)), ":", true);
}

void EthernetFrame::srcAddr(std::string const& mac) {
   setMacAddr(_header->ether_shost, mac);
}

void EthernetFrame::dstAddr(std::string const& mac) {
   setMacAddr(_header->ether_dhost, mac);
}

void EthernetFrame::setMacAddr(unsigned char* buff, std::string const& mac) {
    if (std::sscanf(mac.c_str(),
        "%02s:%02s:%02s:%02s:%02s:%02s",
        &buff[0], &buff[1], &buff[2],
        &buff[3], &buff[4], &buff[5]) != 6) {
        throw std::runtime_error(mac + std::string(" is an invalid MAC address"));
    }
}

}
