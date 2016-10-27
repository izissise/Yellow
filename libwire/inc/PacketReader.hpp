#ifndef PacketReader_hpp
#define PacketReader_hpp

#include "NetInterfaceInfo.h"

namespace Net {

class PacketReader {
public:
    ~PacketReader() = default;

    virtual void startListening(Net::InterfaceInfo const& interface) = 0;
    virtual void stopListening() = 0;
    virtual void nextPacket() = 0;
};

}

#endif /* PacketReader_hpp */
