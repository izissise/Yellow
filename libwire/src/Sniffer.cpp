//
//  Sniffer.cpp
//  Yellow
//
//  Created by Remi Robert on 16/10/2016.
//
//

#include "Sniffer.hpp"

namespace Net {
    Sniffer::Sniffer(PacketReader *packetReader) : _packetReader(packetReader) {}
 
    int Sniffer::startListening(InterfaceInfo const &interface) {
        _packetReader->startListening(interface.getName());
        return 0;
    }
    
    void Sniffer::notifyObserver() {
        for(std::vector<SnifferObserver *>::iterator it = _observers.begin(); it != _observers.end(); ++it) {
            (*it)->update();
        }
    }
}
