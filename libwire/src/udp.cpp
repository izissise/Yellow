#include "udp.h"

#include <netinet/in.h>

namespace Net {

Udp::Udp(data_t const& buffer)
: _udpStruct(*(reinterpret_cast<struct udphdr const*>(buffer.data()))) {
}
void Udp::debugDisplay() {
    std::cout << "UDP Header" << std::endl;
    std::cout << "   |-Source Port      : " << ntohs(_udpStruct.source) << std::endl;
    std::cout << "   |-Destination Port : " << ntohs(_udpStruct.dest) << std::endl;
    std::cout << "   |-UDP Length       : " << ntohs(_udpStruct.len) << std::endl;
    std::cout << "   |-UDP Checksum     : " << ntohs(_udpStruct.check) << std::endl;
}

}
