#ifndef ETHERNETFRAME_H
#define ETHERNETFRAME_H

#include <string>
#include <memory>

#include <net/ethernet.h>

#include "IPacketComposer.h"

#include "Utils.h"

namespace Net {

class EthernetFrame : public IPacketComposer {
    constexpr static size_t HeaderSize = sizeof(ether_header);
public:
    //! @throw WrongSize
    EthernetFrame(data_slice_t const& slice);
    virtual ~EthernetFrame() = default;

    EthernetFrame(EthernetFrame const& o) = delete;
    EthernetFrame& operator=(EthernetFrame const& o) = delete;

    std::string srcAddr() const;
    std::string dstAddr() const;

    void srcAddr(std::string const& mac);
    void dstAddr(std::string const& mac);

    uint16_t type() const { return ntohs(_header->ether_type); }
    void type(uint16_t type) { _header->ether_type = htons(type); };

private:
    void setMacAddr(unsigned char* buff, std::string const& mac);

protected:
    data_slice_t getHeaderBasePtr() const override {
        return data_slice_t(reinterpret_cast<uint8_t*>(_header), HeaderSize);
    }

private:
    ether_header* _header;
};

}

#endif // ETHERNETFRAME_H
