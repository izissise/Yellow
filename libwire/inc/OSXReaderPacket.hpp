//
//  OSXReaderPacket.hpp
//  Yellow
//
//  Created by Remi Robert on 16/10/2016.
//
//

#ifndef OSXReaderPacket_hpp
#define OSXReaderPacket_hpp

#include <stdio.h>
#include <iostream>
#include "PacketReader.hpp"

namespace Net {
    class OSXPacketReader: public PacketReader {
    public:
        void startListening(std::string const interfaceName);
        void stopListening();
        void nextPacket();

    protected:
        int _bpf = -1;
    };
}

#endif /* OSXReaderPacket_hpp */
