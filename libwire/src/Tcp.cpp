#include "Tcp.h"

#include <netinet/in.h>

namespace Net {

Tcp::Tcp(uint8_t* buffer, size_t buffsize)
: ATransport(buffer, buffsize) {
    if (buffsize < sizeof(tcphdr)) {
        throw WrongSize("Error parsing tcp header.", sizeof(tcphdr) - buffsize, sizeof(tcphdr));
    }
    _tcpHeader = reinterpret_cast<tcphdr*>(buffer);
    buffer = &(buffer[sizeof(tcphdr)]);
    buffsize -= sizeof(tcphdr);
    _data = buffer;
    _dataSize = buffsize;
}

}
