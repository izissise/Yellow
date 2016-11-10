#include "IpHeader.h"

#include <iostream>
#include <cstring>

#include <arpa/inet.h>
#include <net/ethernet.h>

namespace Net {

template<>
IpHeader<iphdr>::IpHeader(data_slice_t const& slice) {
    if (slice.size() < HeaderSize) {
        throw WrongSize("Error parsing ipv4 header.", HeaderSize - slice.size(), HeaderSize);
    }
    _header = reinterpret_cast<iphdr*>(slice.ptr());

    // Read options field
    size_t headerAdOptionsSize = headerSizeInBytes();
    if (slice.size() < headerAdOptionsSize) {
        throw WrongSize("Error parsing ipv4 header.", headerAdOptionsSize - slice.size(), headerAdOptionsSize);
    }
}

template<>
IpHeader<ip6_hdr>::IpHeader(data_slice_t const& slice) {
    if (slice.size() < HeaderSize) {
        throw WrongSize("Error parsing ipv6 header.", HeaderSize - slice.size(), HeaderSize);
    }
    _header = reinterpret_cast<ip6_hdr*>(slice.ptr());
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

}
