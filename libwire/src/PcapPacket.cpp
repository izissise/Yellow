#include "PcapPacket.h"

namespace Net {

data_t PcapPacket::getRawData() const {
    return data_t(reinterpret_cast<const char*>(&_header), sizeof(_header)) + _packet;
}

}
