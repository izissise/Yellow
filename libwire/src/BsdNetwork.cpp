#include "BsdNetwork.hpp"

#include <csignal>

#include "LinuxRawSocket.h"

namespace Net {

std::atomic<bool> BsdNetwork::_init(true);

BsdNetwork::BsdNetwork()
{
  if (_init.exchange(false))
    signal(SIGPIPE, SIG_IGN);

  _maxFd = 0;
}

BsdNetwork::~BsdNetwork()
{
}

void BsdNetwork::setFdSet()
{
  _maxFd = 0;
  FD_ZERO(&_setr);
  FD_ZERO(&_setw);

  auto setupdate = [this](int fd) {
    FD_SET(fd, &_setr);
     _maxFd = fd;
  };

  _rawSockets.erase(std::remove_if(_rawSockets.begin(), _rawSockets.end(),
    [&setupdate](std::weak_ptr<ARawSocket>& li) -> bool {
        std::shared_ptr<LinuxRawSocket> sock = std::static_pointer_cast<LinuxRawSocket>(li.lock());
    if (!sock || sock->getFd() == -1)
      return true;
    setupdate(sock->getFd());
    return false;
  }), _rawSockets.end());
}

void BsdNetwork::pollFdsets()
{
    for (auto& ra : _rawSockets)
    {
        std::shared_ptr<LinuxRawSocket> sock = std::static_pointer_cast<LinuxRawSocket>(ra.lock());
        if (sock && sock->getFd() != -1)
            {
                if (FD_ISSET(sock->getFd(), &_setr))
                    dispatchEvent(sock);
            }
    }
}

void BsdNetwork::poll(bool block)
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 1;

  setFdSet();
  if (_rawSockets.size())
    return;
  if ((select(_maxFd + 1, &_setr, &_setw, nullptr, block ? nullptr : &tv) == -1) && errno != EINTR)
    throw lastSystemError();
  pollFdsets();
}

}

