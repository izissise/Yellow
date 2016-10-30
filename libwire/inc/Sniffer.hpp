#ifndef Sniffer_hpp
#define Sniffer_hpp

#include <iostream>
#include <vector>
#include <memory>

#include "SnifferObserver.hpp"
#include "NetInterfaceInfo.h"
#include "PacketReader.hpp"

namespace Net {

class Sniffer {

public:
    explicit Sniffer(std::unique_ptr<PacketReader> packetReader);
    ~Sniffer() = default;
    void startListening(InterfaceInfo const& interface);
    void stopListening();
    Net::Packet nextPacket() const;

private:
    void notifyObserver();

private:
    std::vector<SnifferObserver*> _observers;

protected:
    std::unique_ptr<PacketReader> _packetReader;
};

}

#endif /* Sniffer_hpp */
