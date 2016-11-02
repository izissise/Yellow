#ifndef BSDNETWORK_H
#define BSDNETWORK_H

#include <atomic>

#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <fcntl.h>

#include "ANetwork.hpp"

namespace Net {

class BsdNetwork : public ANetwork {
private:
  static std::atomic<bool> _init;

public:
  BsdNetwork();
  virtual ~BsdNetwork();

  void poll(bool block) override;

protected:
  void setFdSet();
  void pollFdsets();

protected:
  fd_set	_setr;
  fd_set	_setw;
  int		_maxFd;
};

}

#endif // BSDNETWORK_H
