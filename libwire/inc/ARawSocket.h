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
    enum Version {
        V4 = AF_INET,
        V6 = AF_INET6
    };

    enum Protocol {
        TCP = IPPROTO_TCP,
        UDP = IPPROTO_UDP,
        ICMP = IPPROTO_ICMP,
        RAW = IPPROTO_RAW
    };

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
