#include "Packet.h"

#include "IpHeader.h"
#include "Udp.h"
#include "Tcp.h"

namespace Net {

Packet::Packet(data_t const& buffer)
: Net::PcapPacket(buffer), _ethernetHeader(data_slice_t(_packet)) {
    auto slice = nextSlice(_ethernetHeader.getHeaderBasePtr());
//NetworkLayerStuff
    switch(_ethernetHeader.type()) {
        case ETHERTYPE_IP:
            _networkHeader.reset(networkLayerCreate(Version::V4, subSlice);
            break;
        case ETHERTYPE_IPV6:
            _networkHeader.reset(networkLayerCreate(Version::V6, subSlice);
            break;
        default:
            _networkHeader.reset(networkLayerCreate(Version::V4, subSlice);
            break;
    }
    slice = nextSlice(_networkHeader->getHeaderBasePtr());
//TransportLayerStuff

//     _userData = nextSlice(_networkHeader->getHeaderBasePtr());
}

Packet::Packet(Packet const& o)
: Net::PcapPacket(o.packet()), _ethernetHeader(data_slice_t(_packet))) {
}

data_slice_t Packet::nextSlice(data_slice_t const& current) const {
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
