#include "ATransport.h"

namespace Net {

ATransport::ATransport(uint8_t* data, size_t dataSize)
: _data(data), _dataSize(dataSize) {
}

}
