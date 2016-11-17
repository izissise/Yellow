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

class INetworkFrame : public IPacketComposer {
public:
    virtual ~INetworkFrame() = default;

    virtual Net::Version version() const = 0;

    virtual Net::NetAddr srcAddr() const = 0;
    virtual Net::NetAddr dstAddr() const = 0;

    //! @throw std::system_error
    //! @throw std::runtime_error
    virtual void srcAddr(Net::NetAddr const& addr) = 0;
    //! @throw std::system_error
    //! @throw std::runtime_error
    virtual void dstAddr(Net::NetAddr const& addr) = 0;

    virtual size_t hopLimit() const = 0;

    virtual uint8_t nextHeader() const = 0;
    virtual void nextHeader(uint8_t type) = 0;

protected:
    virtual data_slice_t getHeaderBasePtr() const = 0;
};

class IpHeaderV4 : public INetworkFrame {
    constexpr static size_t HeaderSize = sizeof(iphdr);
public:
    //! @throw WrongSize
    IpHeaderV4(data_slice_t const& slice);
    virtual ~IpHeaderV4() = default;

    IpHeaderV4(IpHeaderV4 const& o) = delete;
    IpHeaderV4& operator=(IpHeaderV4 const& o) = delete;

    Net::Version version() const override;

    Net::NetAddr srcAddr() const override;
    Net::NetAddr dstAddr() const override;

    void srcAddr(Net::NetAddr const& addr) override;
    void dstAddr(Net::NetAddr const& addr) override;

    uint8_t nextHeader() const override;
    void nextHeader(uint8_t type) override;

    size_t hopLimit() const override;

    size_t headerSizeInBytes() const;
    int8_t tos() const;
    int16_t tot_len() const;
    int16_t id() const;
    int16_t frag_off() const;
    int16_t check() const;
    data_slice_t getOptionsField() const;

protected:
    data_slice_t getHeaderBasePtr() const override {
        return data_slice_t(reinterpret_cast<uint8_t*>(_header), headerSizeInBytes());
    }

private:
    iphdr* _header;
};


class IpHeaderV6 : public INetworkFrame {
    constexpr static size_t HeaderSize = sizeof(ip6_hdr);
public:
    //! @throw WrongSize
    IpHeaderV6(data_slice_t const& slice);
    virtual ~IpHeaderV6() = default;

    IpHeaderV6(IpHeaderV6 const& o) = delete;
    IpHeaderV6& operator=(IpHeaderV6 const& o) = delete;

    Net::Version version() const override;

    Net::NetAddr srcAddr() const override;
    Net::NetAddr dstAddr() const override;

    void srcAddr(Net::NetAddr const& addr) override;
    void dstAddr(Net::NetAddr const& addr) override;

    uint8_t nextHeader() const override;
    void nextHeader(uint8_t type) override;

    size_t hopLimit() const override;

    uint32_t flow() const;
    uint16_t payloadLength() const;

protected:
    data_slice_t getHeaderBasePtr() const override {
        return data_slice_t(reinterpret_cast<uint8_t*>(_header), HeaderSize);
    }

private:
    ip6_hdr* _header;
};

}

#endif // IPHEADER_H
