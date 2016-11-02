#ifndef ANETWORK_H
#define ANETWORK_H

#include <memory>
#include <map>
#include <deque>
#include <algorithm>

#include "ARawSocket.h"

#include "Utils.h"

namespace Net {

class ANetwork {
public:
  ANetwork() = default;
  virtual ~ANetwork() = default;

  //Raw Sockets
  virtual void registerRawSocket(std::weak_ptr<Net::ARawSocket> const& rawSock);

  virtual void poll(bool block = false) = 0;

protected:
    void dispatchEvent(std::shared_ptr<ARawSocket> const& listener);

protected:
    std::deque<std::weak_ptr<Net::ARawSocket>> _rawSockets;
};

}

#endif // ANETWORK_H
