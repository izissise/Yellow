#include "LinuxNetwork.hpp"

#include <exception>
#include <stdexcept>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <csignal>

#include "LinuxRawSocket.h"

#include "Utils.h"

namespace Net {

std::atomic<bool> LinuxNetwork::_init(true);

LinuxNetwork::LinuxNetwork(size_t maxEvents)
  : _maxEvents(maxEvents)
{
  if (_init.exchange(false))
    signal(SIGPIPE, SIG_IGN);

  _events = new struct epoll_event[_maxEvents];
  _pollFd = epoll_create1(0);
  if (_pollFd == -1)
    throw lastSystemError();
}

LinuxNetwork::~LinuxNetwork()
{
  delete[] _events;
  close(_pollFd);
}

void LinuxNetwork::registerRawSocket(std::weak_ptr<Net::ARawSocket> const& rawSock) {
    std::shared_ptr<LinuxRawSocket> rSock = std::static_pointer_cast<LinuxRawSocket>(rawSock.lock());
  struct epoll_event ev;

  if (!rSock)
    throw std::runtime_error("LinuxNetwork::registerClient");

  std::memset(&ev, 0, sizeof(decltype(ev)));
  ev.data.ptr = (rawSock.lock()).get();
  int ret = epoll_ctl(_pollFd, EPOLL_CTL_ADD, rSock->getFd(), &ev);
  if (ret == -1)
    throw lastSystemError();
  ANetwork::registerRawSocket(rawSock);
}

void LinuxNetwork::updateRequest()
{
  auto epupdate = [this](int fd, void* data) {
    struct epoll_event ev;
    std::memset(&ev, 0, sizeof(decltype(ev)));
    ev.data.ptr = data;
    ev.events = 0;
    ev.events |= EPOLLIN;
    epoll_ctl(_pollFd, EPOLL_CTL_MOD, fd, &ev);
  };

  _rawSockets.erase(std::remove_if(_rawSockets.begin(), _rawSockets.end(),
  [&epupdate](std::weak_ptr<ARawSocket>& rs) -> bool {
      std::shared_ptr<LinuxRawSocket> sock = std::static_pointer_cast<LinuxRawSocket>(rs.lock());
    if (!sock || sock->getFd() == -1)
      return true;
    epupdate(sock->getFd(), (rs.lock()).get());
    return false;
  }), _rawSockets.end());
}

void LinuxNetwork::dispatchEpollEvent(struct epoll_event* ev)
{
  void* ptr = ev->data.ptr;

  for (auto& rs : _rawSockets)
    {
      std::shared_ptr<ARawSocket> sock(rs.lock());
      if (sock && (sock.get() == ptr))
        {
          if (ev->events & EPOLLIN)
              dispatchEvent(sock);
        }
    }
}

void LinuxNetwork::poll(bool block)
{
  updateRequest();
  if (_rawSockets.size() == 0)
    return;
  int ret = epoll_wait(_pollFd, _events, static_cast<int>(_maxEvents), block ? -1 : 0);
  if (ret == -1 && errno != EINTR)
    throw lastSystemError();
  for (int i = 0; i < ret; ++i)
      dispatchEpollEvent(&(_events[i]));
}

}
