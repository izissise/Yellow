#ifndef PacketReader_hpp
#define PacketReader_hpp

#include "NetInterfaceInfo.h"
#include "packet.h"

namespace Net {

class PacketReader {
public:
    ~PacketReader() = default;

    virtual void startListening(Net::InterfaceInfo const& interface) = 0;
    virtual void stopListening() = 0;
    virtual Net::Packet nextPacket() const = 0;
};

}

#endif /* PacketReader_hpp */
