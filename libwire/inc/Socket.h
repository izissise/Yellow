#ifndef SOCKET_H
#define SOCKET_H

#include "NetInterfaceInfo.h"

#include "Utils.h"

namespace Net {

class ISocket {
public:
    virtual ~ISocket() = default;

    //! @throw std::system_error
    virtual void startListening(Net::InterfaceInfo const& interface) = 0;

    //! @throw std::system_error
    virtual void stopListening() = 0;

    virtual bool isListening() const = 0;
    virtual int getFd() const = 0;
};

template<class PROTOCOL>
class Socket : public ISocket {
public:
    Socket();
    virtual ~Socket();

    //! @throw std::system_error
    void startListening(Net::InterfaceInfo const& interface) override;

    //! @throw std::system_error
    void stopListening() override;

    bool isListening() const override { return _listening; }
    int getFd() const override { return _fd; }
private:
    bool _listening;
    int _fd;
};

template<class PROTOCOL>
Socket<PROTOCOL>::Socket()
: _listening(false), _fd(-1) {
}

template<class PROTOCOL>
Socket<PROTOCOL>::~Socket() {
    stopListening();
}

template<class PROTOCOL>
void Socket<PROTOCOL>::startListening(Net::InterfaceInfo const& interface) {
    if (_listening)
        return;
    _fd = socket(AF_INET, SOCK_RAW, PROTOCOL::UnixId);
    if (_fd < 0) {
        throw lastSystemError();
    }
    if (setsockopt(_fd, SOL_SOCKET, SO_BINDTODEVICE, interface.getName().addr().c_str(), 4) < 0) {
        auto err = lastSystemError();
        close(_fd);
        throw err;
    }
    _listening = true;
}

template<class PROTOCOL>
void Socket<PROTOCOL>::stopListening() {
    if (!_listening)
        return;
    if (close(_fd) < 0)
        throw lastSystemError();
    _listening = false;
}

}

#endif // SOCKET_H
