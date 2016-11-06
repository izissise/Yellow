#include "ARawSocket.h"

namespace Net {

ARawSocket::ARawSocket(std::function<void (data_t const& data)> readCallback)
: _readCb(readCallback) {
}

void ARawSocket::onReadAvailable() const {
  _readCb(_readSock());
}

}
