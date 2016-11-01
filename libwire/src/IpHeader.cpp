#include "IpHeader.h"

#include <iostream>
#include <cstring>

#include <arpa/inet.h>

namespace Net {

IpHeader::IpHeader(data_t const& buffer) {
    if (buffer.length() < sizeof(iphdr_t)) {
      throw WrongSize("Error parsing ip header", sizeof(iphdr_t) - buffer.length(), sizeof(iphdr_t));
    }
    _ipHeader = *(reinterpret_cast<const iphdr_t*>(buffer.data()));
    struct sockaddr_storage source, dest;
    struct sockaddr_in* ipv4ptr;

    std::memset(&source, 0, sizeof(source));
    std::memset(&dest, 0, sizeof(dest));

    ipv4ptr = reinterpret_cast<struct sockaddr_in*>(&source);
    ipv4ptr->sin_addr.s_addr = _ipHeader.saddr;
    ipv4ptr = reinterpret_cast<struct sockaddr_in*>(&dest);
    ipv4ptr->sin_addr.s_addr = _ipHeader.daddr;

    try {
        _saddr = reinterpret_cast<struct sockaddr*>(&source);
        _daddr = reinterpret_cast<struct sockaddr*>(&dest);
    } catch (std::system_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

IpHeader::~IpHeader() {}
bool IpHeader::operator==(const IpHeader& other) const { (void)other; return true; }

void IpHeader::debugDisplay() {
    std::cout << "   |-IP Version          " << static_cast<unsigned int>(_ipHeader.version) << "\n"
              << "   |-IP Header Length  : " << static_cast<unsigned int>(_ipHeader.ihl) << "\n"
              << "   |-Type Of Service   : " << static_cast<unsigned int>(_ipHeader.tos) << "\n"
              << "   |-IP Total Length   : " << static_cast<unsigned int>(_ipHeader.tot_len) << "\n"
              << "   |-Identification    : " << static_cast<unsigned int>(_ipHeader.id) << "\n"
              << "   |-TTL               : " << static_cast<unsigned int>(_ipHeader.ttl) << "\n"
              << "   |-Protocol          : " << static_cast<unsigned int>(_ipHeader.protocol) << "\n"
              << "   |-Checksum          : " << static_cast<unsigned int>(_ipHeader.check) << "\n"
              << "   |-Source IP         : " << _saddr << "\n"
              << "   |-Destination IP    : " << _daddr << std::endl;
}

}
