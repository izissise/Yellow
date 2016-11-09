#include "Udp.h"

#include <netinet/in.h>

namespace Net {

Udp::Udp(uint8_t* buffer, size_t buffsize)
: ATransport(buffer, buffsize) {
    if (buffsize < sizeof(udphdr)) {
        throw WrongSize("Error parsing udp header.", sizeof(udphdr) - buffsize, sizeof(udphdr));
    }
    _udpHeader = reinterpret_cast<udphdr*>(buffer);
    buffer = &(buffer[sizeof(udphdr)]);
    buffsize -= sizeof(udphdr);
    _data = buffer;
    _dataSize = buffsize;
}

}
