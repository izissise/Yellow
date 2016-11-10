#include "Udp.h"

#include <netinet/in.h>

namespace Net {

Udp::Udp(data_slice_t const& slice) {
    if (slice.size() < HeaderSize) {
        throw WrongSize("Error parsing udp header.", HeaderSize - slice.size(), HeaderSize);
    }
    _udpHeader = reinterpret_cast<udphdr*>(slice.ptr());
}

}
