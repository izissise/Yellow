#ifndef ETHERNETFRAME_H
#define ETHERNETFRAME_H

#include <string>
#include <memory>

#include <net/ethernet.h>

#include "Placement_ptr.h"

#include "IpHeader.h"
#include "Utils.h"

namespace Net {

class EthernetFrame {
public:
    //! @throw WrongSize
    EthernetFrame(uint8_t* buffer, size_t buffsize);
    virtual ~EthernetFrame() = default;

    EthernetFrame(EthernetFrame const& o) = delete;
    EthernetFrame& operator=(EthernetFrame const& o) = delete;

    std::string srcAddr() const;
    std::string dstAddr() const;

    uint16_t type() const { return switchEndianness(_header->ether_type); }

    const Net::IIpHeader* getNetworkLayer() const { return _ipHeader.get(); }
    Net::IIpHeader* getNetworkLayer(){ return _ipHeader.get(); }

protected:
    ether_header* _header;

// Sub protocol
protected:
    placement_ptr<Net::IIpHeader, networkLayerStorageSize(), networkLayerAlignSize()> _ipHeader;
};

}

#endif // ETHERNETFRAME_H
