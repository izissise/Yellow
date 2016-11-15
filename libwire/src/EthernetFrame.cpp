#include "EthernetFrame.h"

#include <array>
#include <cstring>

namespace Net {

EthernetFrame::EthernetFrame(data_slice_t const& slice) {
    if (slice.size() < HeaderSize) {
        throw WrongSize("Error parsing ethernet header.", HeaderSize - slice.size(), HeaderSize);
    }
    _header = reinterpret_cast<ether_header*>(slice.ptr());
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
    unsigned int tmpBuff[6];
    char c;
    if (std::sscanf(mac.c_str(),
        "%x:%x:%x:%x:%x:%x%c",
        &tmpBuff[0], &tmpBuff[1], &tmpBuff[2],
        &tmpBuff[3], &tmpBuff[4], &tmpBuff[5], &c) != 6) {
        throw std::runtime_error(mac + std::string(" is an invalid MAC address"));
    }
    for (size_t i = 0; i < 6; ++i) {
        buff[i] = static_cast<uint8_t>(tmpBuff[i]);
    }
}

}
