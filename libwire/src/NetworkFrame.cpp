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

void IpHeaderV4::srcAddr(Net::NetAddr const& addr) {
    _header->saddr = addr.addrGetRawV4();
}

Net::NetAddr IpHeaderV6::srcAddr() const {
    return Net::NetAddr(_header->ip6_src);
}

void IpHeaderV6::srcAddr(Net::NetAddr const& addr) {
    _header->ip6_src = addr.addrGetRawV6();
}

Net::NetAddr IpHeaderV4::dstAddr() const {
    return Net::NetAddr(_header->daddr);
}

void IpHeaderV4::dstAddr(Net::NetAddr const& addr) {
    _header->daddr = addr.addrGetRawV4();
}

Net::NetAddr IpHeaderV6::dstAddr() const {
    return Net::NetAddr(_header->ip6_dst);
}

void IpHeaderV6::dstAddr(Net::NetAddr const& addr) {
    _header->ip6_dst = addr.addrGetRawV6();
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

int16_t IpHeaderV4::check() const {
    return ntohs(_header->check);
}

uint8_t IpHeaderV4::nextHeader() const {
    return _header->protocol;
}

void IpHeaderV4::nextHeader(uint8_t type) {
    _header->protocol = type;
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

void IpHeaderV6::nextHeader(uint8_t type) {
    _header->ip6_ctlun.ip6_un1.ip6_un1_nxt = type;
}

}
