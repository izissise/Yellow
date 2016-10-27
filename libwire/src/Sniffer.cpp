#include "Sniffer.hpp"

namespace Net {

Sniffer::Sniffer(std::unique_ptr<PacketReader> packetReader)
: _packetReader(std::move(packetReader)) {
}

void Sniffer::startListening(InterfaceInfo const& interface) {
    _packetReader->startListening(interface);
}

void Sniffer::stopListening() {
    _packetReader->stopListening();
}

void Sniffer::notifyObserver() {
    for (auto& obs : _observers) {
       obs->update();
    }
}

}
