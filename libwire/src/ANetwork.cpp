#include "ANetwork.hpp"

namespace Net {

void ANetwork::registerRawSocket(std::weak_ptr<Net::ARawSocket> const& rawSock) {
    _rawSockets.push_front(rawSock);
}

void ANetwork::dispatchEvent(std::shared_ptr<ARawSocket> const& rawSock) {
    rawSock->onReadAvailable();
}

}
