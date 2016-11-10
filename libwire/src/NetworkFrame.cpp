#include "NetworkFrame.h"

#include <iostream>
#include <cstring>

#include <arpa/inet.h>
#include <net/ethernet.h>

namespace Net {

IpHeaderV4::IpHeaderV4(data_slice_t const& slice) {
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

IpHeaderV6::IpHeaderV6(data_slice_t const& slice) {
    if (slice.size() < HeaderSize) {
        throw WrongSize("Error parsing ipv6 header.", HeaderSize - slice.size(), HeaderSize);
    }
    _header = reinterpret_cast<ip6_hdr*>(slice.ptr());
}

Net::Version IpHeaderV4::version() const {
    return Version::V4;
}

Net::Version IpHeaderV6::version() const {
    return Version::V6;
}

Net::NetAddr IpHeaderV4::srcAddr() const {
   return Net::NetAddr(_header->saddr);
}

Net::NetAddr IpHeaderV6::srcAddr() const {
    return Net::NetAddr(_header->ip6_src);
}

Net::NetAddr IpHeaderV4::dstAddr() const {
    return Net::NetAddr(_header->daddr);
}

Net::NetAddr IpHeaderV6::dstAddr() const {
    return Net::NetAddr(_header->ip6_dst);
}

size_t IpHeaderV4::hopLimit() const {
  return _header->ttl;
}

size_t IpHeaderV6::hopLimit() const {
    return _header->ip6_ctlun.ip6_un1.ip6_un1_hlim;
}

size_t IpHeaderV4::headerSizeInBytes() const {
    return (_header->ihl) * 4;
}

int8_t IpHeaderV4::tos() const {
    return _header->tos;
}

int16_t IpHeaderV4::tot_len() const {
    return ntohs(_header->tot_len);
}

int16_t IpHeaderV4::id() const {
    return ntohs(_header->id);
}

int16_t IpHeaderV4::frag_off() const {
    return ntohs(_header->frag_off);
}

int8_t IpHeaderV4::protocol() const {
    return _header->protocol;
}

int16_t IpHeaderV4::check() const {
    return ntohs(_header->check);
}

uint8_t IpHeaderV4::nextHeader() const {
    auto slice = getOptionsField();
    if (slice.ptr() == nullptr || slice.size() == 0) {
       std::cerr << "No ipv4 options field defaulting to tcp" << std::endl;
       return IPPROTO_TCP;
    }
    return slice.ptr()[0];
}

data_slice_t IpHeaderV4::getOptionsField() const {
    uint8_t* ptr = reinterpret_cast<uint8_t*>(_header);
    size_t headerAdOptionsSize = headerSizeInBytes();
    if (headerAdOptionsSize < 6)
        return data_slice_t();
    ptr = &(ptr[HeaderSize]);
    return data_slice_t(ptr, headerAdOptionsSize - HeaderSize);
}

uint32_t IpHeaderV6::flow() const {
    return ntohl(_header->ip6_ctlun.ip6_un1.ip6_un1_flow);
}

uint16_t IpHeaderV6::payloadLength() const {
    return ntohs(_header->ip6_ctlun.ip6_un1.ip6_un1_plen);
}

uint8_t IpHeaderV6::nextHeader() const {
    return _header->ip6_ctlun.ip6_un1.ip6_un1_nxt;
}

}
