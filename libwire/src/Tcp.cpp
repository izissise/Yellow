#include "Tcp.h"

#include <netinet/in.h>

namespace Net {

Tcp::Tcp(data_t const& buffer)
: _tcpStruct(*(reinterpret_cast<struct tcphdr const*>(buffer.data()))) {
}

void Tcp::debugDisplay() {
    std::cout << "TCP Header" << "\n"
              << "   |-Source Port        : " << ntohs(_tcpStruct.th_sport) << "\n"
              << "   |-Destination Port   : " << ntohs(_tcpStruct.th_dport) << "\n"
              << "   |-Sequence number    : " << ntohs(_tcpStruct.th_seq) << "\n"
              << "   |-Acknowledge Number : " << ntohs(_tcpStruct.th_ack) << "\n"
              << "   |-Header Length      : " << ntohs(_tcpStruct.th_off) << "\n"
              << "   |-Flags              : " << ntohs(_tcpStruct.th_flags) << std::endl;
}

}
