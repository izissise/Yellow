#include "Packet.h"

namespace Net {

Packet::Packet(data_t const& buffer)
: Net::PcapPacket(buffer), _ethernetHeader(data_slice_t(_packet)) {
    auto eth = ethernetFrame();
    auto slice = nextSlice(static_cast<IPacketComposer*>(eth)->getHeaderBasePtr());
//NetworkLayerStuff
    switch(eth->type()) {
        case ETHERTYPE_IP:
            _networkHeader.reset(placement_ptr<IpHeaderV4, decltype(_networkHeader)::StoreSize, decltype(_networkHeader)::StoreAlign>(slice));
            break;
        case ETHERTYPE_IPV6:
            _networkHeader.reset(placement_ptr<IpHeaderV6, decltype(_networkHeader)::StoreSize, decltype(_networkHeader)::StoreAlign>(slice));
            break;
        default:
            _networkHeader.reset(placement_ptr<IpHeaderV4, decltype(_networkHeader)::StoreSize, decltype(_networkHeader)::StoreAlign>(slice));
            break;
    }
    slice = nextSlice(static_cast<IPacketComposer*>(_networkHeader.get())->getHeaderBasePtr());
//TransportLayerStuff
    switch(_networkHeader->nextHeader()) {
        case IPPROTO_TCP:
            _transportHeader.reset(placement_ptr<Tcp, decltype(_transportHeader)::StoreSize, decltype(_transportHeader)::StoreAlign>(slice));
            break;
        case IPPROTO_UDP:
            _transportHeader.reset(placement_ptr<Udp, decltype(_transportHeader)::StoreSize, decltype(_transportHeader)::StoreAlign>(slice));
            break;
        case IPPROTO_ICMP:
            _transportHeader.reset(placement_ptr<Icmp, decltype(_transportHeader)::StoreSize, decltype(_transportHeader)::StoreAlign>(slice));
            break;
        default:
            _transportHeader.reset(placement_ptr<Udp, decltype(_transportHeader)::StoreSize, decltype(_transportHeader)::StoreAlign>(slice));
            break;
    }
    _userData = nextSlice(static_cast<IPacketComposer*>(_transportHeader.get())->getHeaderBasePtr());
}

Packet::Packet(Net::Packet const& o)
: Net::PcapPacket(o.packet()), _ethernetHeader(data_slice_t(_packet)) {
}

data_slice_t Packet::nextSlice(data_slice_t const& current) {
    data_slice_t bigSlice(_packet);

    auto nextPtr = &(current.ptr()[current.size()]);
    size_t nextSize = bigSlice.size() - (nextPtr - bigSlice.ptr());
    return data_slice_t(nextPtr, nextSize);
}

// Packet& Packet::operator=(Packet const& o) {
//     setPacket(o.packet());
//     setDate();
//     EthernetFrame frame(const_cast<uint8_t*>(_packet.data()), _packet.size());
// //     std::move(_ethernetHeader, frame);
//     return *this;
// }

}
