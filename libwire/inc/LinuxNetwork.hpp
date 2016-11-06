#ifndef LUNIXNETWORK_H
#define LUNIXNETWORK_H

#include <atomic>

#include <sys/epoll.h>

#include "ANetwork.hpp"

namespace Net {

class LinuxNetwork : public ANetwork
{
private:
  static std::atomic<bool>   _init;

public:
  LinuxNetwork(size_t maxEvents = 200);
  virtual ~LinuxNetwork();

  //Raw Sockets
  void registerRawSocket(std::weak_ptr<Net::ARawSocket> const& rawSock) override;

  void poll(bool block) override;

protected:
  void updateRequest();
  void dispatchEpollEvent(struct epoll_event* ev);

protected:
  int _pollFd;
  size_t _maxEvents;
  struct epoll_event* _events;
};

}

#endif // UNIXNETWORK_H
