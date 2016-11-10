#ifndef PACKET_H
#define PACKET_H

#include <algorithm>

#include "IpHeader.h"
#include "PcapPacket.h"
#include "NetUtils.h"
#include "Placement_ptr.h"
#include "Placement_ptrHelper.h"
#include "DataSlice.h"

#include "EthernetFrame.h"

#include "IpHeader.h"

#include "RawData.h"

namespace Net {

class Packet : public PcapPacket {
    using networkLayerPtr = typename Placement_ptrBaseAndDerivedInfos<Net::IIpHeader, IpHeaderV4, IpHeaderV6>::PlacementPtrType;

public:
    Packet(data_t const& buffer);
    virtual ~Packet() = default;

    Packet(Packet const& o);
    Packet& operator=(Packet const& o) = delete;

    const EthernetFrame* ethernetFrame() const { return &_ethernetHeader; };
    EthernetFrame* ethernetFrame() { return &_ethernetHeader; };

    const Net::IIpHeader* getNetworkLayer() const { return _networkHeader.get(); }
    Net::IIpHeader* getNetworkLayer(){ return _networkHeader.get(); }

private:
    data_slice_t nextSlice(data_slice_t const& current);

private:
    EthernetFrame _ethernetHeader;
    networkLayerPtr _networkHeader;

    data_slice_t   _userData;
};

}

#endif // PACKET_H
