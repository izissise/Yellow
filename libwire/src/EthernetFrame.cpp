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
    if (std::sscanf(mac.c_str(),
        "%02s:%02s:%02s:%02s:%02s:%02s",
        &buff[0], &buff[1], &buff[2],
        &buff[3], &buff[4], &buff[5]) != 6) {
        throw std::runtime_error(mac + std::string(" is an invalid MAC address"));
    }
}

}
