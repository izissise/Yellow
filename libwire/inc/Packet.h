#ifndef PACKET_H
#define PACKET_H

#include <algorithm>

#include "PcapPacket.h"
#include "NetUtils.h"
#include "Placement_ptr.h"
#include "Placement_ptrHelper.h"
#include "DataSlice.h"

#include "EthernetFrame.h"
#include "NetworkFrame.h"

#include "ATransport.h"
#include "Tcp.h"
#include "Udp.h"
#include "Icmp.h"

namespace Net {

class Packet : public PcapPacket {
    using networkLayerPtr = typename Placement_ptrBaseAndDerivedInfos<INetworkFrame, IpHeaderV4, IpHeaderV6>::PlacementPtrType;
    using transportLayerPtr = typename Placement_ptrBaseAndDerivedInfos<ATransport, Tcp, Udp, Icmp>::PlacementPtrType;

public:
    Packet(data_t const& buffer);
    virtual ~Packet() = default;

    Packet(Packet const& o);
    Packet& operator=(Packet const& o) = delete;

    const EthernetFrame* ethernetFrame() const { return &_ethernetHeader; };
    EthernetFrame* ethernetFrame() { return &_ethernetHeader; };

    const Net::INetworkFrame* getNetworkLayer() const { return _networkHeader.get(); }
    Net::INetworkFrame* getNetworkLayer() { return _networkHeader.get(); }

    const Net::ATransport* getTransportLayer() const { return _transportHeader.get(); }
    Net::ATransport* getTransportLayer() { return _transportHeader.get(); }

    data_t getPayload() const { return data_t(_userData.ptr(), &(_userData.ptr()[_userData.size()])); }

private:
    data_slice_t nextSlice(data_slice_t const& current);

private:
    EthernetFrame     _ethernetHeader;
    networkLayerPtr   _networkHeader;
    transportLayerPtr _transportHeader;

    data_slice_t   _userData;
};

}

#endif // PACKET_H
