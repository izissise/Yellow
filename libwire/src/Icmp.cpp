#include "Icmp.h"

#include <netinet/in.h>

namespace Net {

Icmp::Icmp(uint8_t* buffer, size_t buffsize)
: ATransport(buffer, buffsize) {
    if (buffsize < sizeof(icmphdr)) {
        throw WrongSize("Error parsing icmp header.", sizeof(icmphdr) - buffsize, sizeof(icmphdr));
    }
    _icmpHeader = reinterpret_cast<icmphdr*>(buffer);
    buffer = &(buffer[sizeof(icmphdr)]);
    buffsize -= sizeof(icmphdr);
    _data = buffer;
    _dataSize = buffsize;
}

}
