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

#include "Placement_ptr.h"

#include "IPacketComposer.h"
#include "NetUtils.h"
#include "Utils.h"

namespace Net {

class IIpHeader : public IPacketComposer {
public:
    virtual ~IIpHeader() = default;

    virtual Net::Version version() const = 0;

    virtual Net::NetAddr srcAddr() const = 0;
    virtual Net::NetAddr dstAddr() const = 0;

    virtual size_t hopLimit() const = 0;

protected:
    virtual data_slice_t getHeaderBasePtr() const = 0;
};

template<typename HeaderStruct>
class IpHeader : public IIpHeader {
    constexpr static size_t HeaderSize = sizeof(HeaderStruct);
public:
    //! @throw WrongSize
    IpHeader(data_slice_t const& slice);
    virtual ~IpHeader() = default;

    IpHeader(IpHeader const& o) = delete;
    IpHeader& operator=(IpHeader const& o) = delete;

    Net::Version version() const override;

    Net::NetAddr srcAddr() const override;
    Net::NetAddr dstAddr() const override;

    size_t hopLimit() const override;

    // IPv4 Specific
    template<typename T = HeaderStruct, typename std::enable_if<std::is_same<T, iphdr>::value, int>::type = 0>
    size_t headerSizeInBytes() const;
    template<typename T = HeaderStruct, typename std::enable_if<std::is_same<T, iphdr>::value, int>::type = 0>
    int8_t tos() const;
    template<typename T = HeaderStruct, typename std::enable_if<std::is_same<T, iphdr>::value, int>::type = 0>
    int16_t tot_len() const;
    template<typename T = HeaderStruct, typename std::enable_if<std::is_same<T, iphdr>::value, int>::type = 0>
    int16_t id() const;
    template<typename T = HeaderStruct, typename std::enable_if<std::is_same<T, iphdr>::value, int>::type = 0>
    int16_t frag_off() const;
    template<typename T = HeaderStruct, typename std::enable_if<std::is_same<T, iphdr>::value, int>::type = 0>
    int8_t protocol() const;
    template<typename T = HeaderStruct, typename std::enable_if<std::is_same<T, iphdr>::value, int>::type = 0>
    int16_t check() const;

    // IPv6 Specific
    template<typename T = HeaderStruct, typename std::enable_if<std::is_same<T, ip6_hdr>::value, int>::type = 0>
    uint32_t flow() const;
    template<typename T = HeaderStruct, typename std::enable_if<std::is_same<T, ip6_hdr>::value, int>::type = 0>
    uint16_t payloadLength() const;
    template<typename T = HeaderStruct, typename std::enable_if<std::is_same<T, ip6_hdr>::value, int>::type = 0>
    uint16_t nextHeader() const;

protected:
    data_slice_t getHeaderBasePtr() const override {
        return data_slice_t(reinterpret_cast<uint8_t*>(_header), HeaderSize);
    }

private:
    HeaderStruct* _header;
};

// IPv4
template<> template<>
inline size_t IpHeader<iphdr>::headerSizeInBytes() const { return (_header->ihl) * 4; }

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

}

#endif // IPHEADER_H
