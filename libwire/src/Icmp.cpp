#include "Icmp.h"

#include <netinet/in.h>

namespace Net {

Icmp::Icmp(data_slice_t const& slice) {
    if (slice.size() < HeaderSize) {
        throw WrongSize("Error parsing icmp header.", HeaderSize - slice.size(), HeaderSize);
    }
    _icmpHeader = reinterpret_cast<icmphdr*>(slice.ptr());
}

}
