#include "IpHeader.h"

#include <iostream>
#include <cstring>

#include <arpa/inet.h>
#include <net/ethernet.h>

namespace Net {

template<>
IpHeader<iphdr>::IpHeader(uint8_t* buffer, size_t buffsize) {
    if (buffsize < sizeof(iphdr)) {
        throw WrongSize("Error parsing ip header.", sizeof(iphdr) - buffsize, sizeof(iphdr));
    }
    _header = reinterpret_cast<iphdr*>(buffer);
    // Read options field
    size_t headerSize = headerSizeInBytes();
    if (buffsize < headerSize) {
        throw WrongSize("Error parsing ip header.", headerSize - buffsize, sizeof(iphdr));
    }
}

template<>
IpHeader<ip6_hdr>::IpHeader(uint8_t* buffer, size_t buffsize) {
    if (buffsize < sizeof(ip6_hdr)) {
        throw WrongSize("Error parsing ip header.", sizeof(ip6_hdr) - buffsize, sizeof(ip6_hdr));
    }
    _header = reinterpret_cast<ip6_hdr*>(buffer);
}

template<>
Net::Version IpHeader<iphdr>::version() const {
    return Version::V4;
}

template<>
Net::Version IpHeader<ip6_hdr>::version() const {
    return Version::V6;
}

template<>
Net::NetAddr IpHeader<iphdr>::srcAddr() const {
   return Net::NetAddr(_header->saddr);
}

template<>
Net::NetAddr IpHeader<ip6_hdr>::srcAddr() const {
    return Net::NetAddr(_header->ip6_src);
}

template<>
Net::NetAddr IpHeader<iphdr>::dstAddr() const {
    return Net::NetAddr(_header->daddr);
}

template<>
Net::NetAddr IpHeader<ip6_hdr>::dstAddr() const {
    return Net::NetAddr(_header->ip6_dst);
}

template<>
size_t IpHeader<iphdr>::hopLimit() const {
  return _header->ttl;
}

template<>
size_t IpHeader<ip6_hdr>::hopLimit() const {
    return _header->ip6_ctlun.ip6_un1.ip6_un1_hlim;
}

template class IpHeader<iphdr>;
template class IpHeader<ip6_hdr>;

Net::IIpHeader* ipHeaderPlacementNew(void* storage, Net::Version version, uint8_t* buffer, size_t buffsize) {
    if (version == Version::V4) {
        return new (storage) IpHeaderV4(buffer, buffsize);
    } else if (version == Version::V6) {
        return new (storage) IpHeaderV6(buffer, buffsize);
    }
    return new (storage) IpHeaderV4(buffer, buffsize);
}

}
