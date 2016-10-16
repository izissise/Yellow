//
//  PacketReader.hpp
//  Yellow
//
//  Created by Remi Robert on 16/10/2016.
//
//

#ifndef PacketReader_hpp
#define PacketReader_hpp

#include <stdio.h>

namespace Net {
    class PacketReader {
    public:
        virtual void startListening(std::string const interfaceName) = 0;
        virtual void stopListening() = 0;
        virtual void nextPacket() = 0;
    };
}

#endif /* PacketReader_hpp */
