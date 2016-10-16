//
//  Sniffer.hpp
//  Yellow
//
//  Created by Remi Robert on 16/10/2016.
//
//

#ifndef Sniffer_hpp
#define Sniffer_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "SnifferObserver.hpp"
#include "NetInterfaceInfo.h"
#include "PacketReader.hpp"

namespace Net {
    class Sniffer {
        
    public:
        explicit Sniffer(PacketReader *packetReader);
        ~Sniffer() = default;
        int startListening(InterfaceInfo const &interface);
        int stopListening();
        
    private:
        std::vector <class SnifferObserver *> _observers;
        void notifyObserver();

    protected:
        PacketReader *_packetReader;
    };
}

#endif /* Sniffer_hpp */
