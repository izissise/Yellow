#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <functional>
#include <memory>

#include <sys/socket.h>
#include <netinet/in.h>

#include "Utils.h"

namespace Net {

class ARawSocket {
public:
    ARawSocket(std::function<void (data_t const& data)> readCallback);
    virtual ~ARawSocket() = default;

    //! @throw std::system_error
    virtual void startSniffing(std::string const& interface) = 0;

    void onReadAvailable() const;

protected:
    virtual data_t _readSock() const = 0;

protected:
    std::function<void (data_t const& data)> _readCb;
};

}

#endif // SOCKET_H
