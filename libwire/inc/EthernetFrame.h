#ifndef ETHERNETFRAME_H
#define ETHERNETFRAME_H

#include <string>
#include <memory>

#include <net/ethernet.h>

#include "IpHeader.h"
#include "Utils.h"

namespace Net {

class EthernetFrame {
public:
    //! @throw WrongSize
    EthernetFrame(uint8_t* buffer, size_t buffsize);
    virtual ~EthernetFrame();

    EthernetFrame(EthernetFrame const& o) = delete;
    EthernetFrame& operator=(EthernetFrame const& o) = delete;

    std::string srcAddr() const;
    std::string dstAddr() const;

    uint16_t type() const { return switchEndianness(_header->ether_type); }

    const Net::IIpHeader* getNetworkLayer() const { return _ipHeader; }
    Net::IIpHeader* getNetworkLayer(){ return _ipHeader; }

protected:
    ether_header* _header;

// Sub protocol
protected:
    Net::IIpHeader* _ipHeader = nullptr;

    // Class Objects storage
private:
    std::aligned_storage<networkLayerStorageSize(), networkLayerAlignSize()>::type _ipHeaderStore;
};

}

#endif // ETHERNETFRAME_H
