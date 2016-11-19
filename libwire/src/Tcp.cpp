#include "Tcp.h"

#include <netinet/in.h>

namespace Net {

Tcp::Tcp(data_slice_t const& slice) {
    if (slice.size() < HeaderSize) {
        throw WrongSize("Error parsing tcp header.", HeaderSize - slice.size(), HeaderSize);
    }
    _tcpHeader = reinterpret_cast<tcphdr*>(slice.ptr());
}

}
