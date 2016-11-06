#ifndef IPHEADER_H
#define IPHEADER_H

#include <string>
#include <type_traits>
#include <memory>

#include <stdint.h>
#include <asm/types.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>

#include "NetUtils.h"
#include "Utils.h"

namespace Net {

class IIpHeader {
public:
    virtual ~IIpHeader() = default;

    virtual Net::Version version() const = 0;

    virtual Net::NetAddr srcAddr() const = 0;
    virtual Net::NetAddr dstAddr() const = 0;

    virtual size_t hopLimit() const = 0;
};

template<typename HeaderStruct>
class IpHeader : public IIpHeader {
public:
    //! @throw WrongSize
    IpHeader(uint8_t* buffer, size_t buffsize);
    virtual ~IpHeader() = default;

    Net::Version version() const override;

    Net::NetAddr srcAddr() const override;
    Net::NetAddr dstAddr() const override;

    size_t hopLimit() const override;

    // IPv4 Specific
    template<typename T = HeaderStruct> typename std::enable_if<std::is_same<T, iphdr>::value, size_t>::type headerSizeInBytes() const;
    template<typename T = HeaderStruct> typename std::enable_if<std::is_same<T, iphdr>::value, int8_t>::type tos() const;
    template<typename T = HeaderStruct> typename std::enable_if<std::is_same<T, iphdr>::value, int16_t>::type tot_len() const;
    template<typename T = HeaderStruct> typename std::enable_if<std::is_same<T, iphdr>::value, int16_t>::type id() const;
    template<typename T = HeaderStruct> typename std::enable_if<std::is_same<T, iphdr>::value, int16_t>::type frag_off() const;
    template<typename T = HeaderStruct> typename std::enable_if<std::is_same<T, iphdr>::value, int8_t>::type protocol() const;
    template<typename T = HeaderStruct> typename std::enable_if<std::is_same<T, iphdr>::value, int16_t>::type check() const;

    // IPv6 Specific
    template<typename T = HeaderStruct> typename std::enable_if<std::is_same<T, ip6_hdr>::value, uint32_t>::type flow() const;
    template<typename T = HeaderStruct> typename std::enable_if<std::is_same<T, ip6_hdr>::value, uint16_t>::type payloadLength() const;
    template<typename T = HeaderStruct> typename std::enable_if<std::is_same<T, ip6_hdr>::value, uint16_t>::type nextHeader() const;


private:
    HeaderStruct* _header;
};

// IPv4
template<> template<>
inline size_t IpHeader<iphdr>::headerSizeInBytes() const { return ntohs(_header->ihl) * 4; }

template<> template<>
inline int8_t IpHeader<iphdr>::tos() const { return _header->tos; }

template<> template<>
inline int16_t IpHeader<iphdr>::tot_len() const { return ntohs(_header->tot_len); }

template<> template<>
inline int16_t IpHeader<iphdr>::id() const { return ntohs(_header->id); }

template<> template<>
inline int16_t IpHeader<iphdr>::frag_off() const { return ntohs(_header->frag_off); }

template<> template<>
inline int8_t IpHeader<iphdr>::protocol() const { return _header->protocol; }

template<> template<>
inline int16_t IpHeader<iphdr>::check() const { return ntohs(_header->check); }

// IPv6
template<> template<>
inline uint32_t IpHeader<ip6_hdr>::flow() const {
    return ntohl(_header->ip6_ctlun.ip6_un1.ip6_un1_flow);
}

template<> template<>
inline uint16_t IpHeader<ip6_hdr>::payloadLength() const {
    return ntohs(_header->ip6_ctlun.ip6_un1.ip6_un1_plen);
}

template<> template<>
inline uint16_t IpHeader<ip6_hdr>::nextHeader() const {
    return ntohs(_header->ip6_ctlun.ip6_un1.ip6_un1_nxt);
}

typedef IpHeader<iphdr> IpHeaderV4;
typedef IpHeader<ip6_hdr> IpHeaderV6;

constexpr size_t networkLayerStorageSize() {
    return std::max(sizeof(IpHeaderV4), sizeof(IpHeaderV6));
}

//! @throw WrongSize
Net::IIpHeader* ipHeaderPlacementNew(void* storage, Net::Version version, uint8_t* buffer, size_t buffsize);

}

#endif // IPHEADER_H
