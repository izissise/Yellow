#ifndef LINUXRAWSOCKET_H
#define LINUXRAWSOCKET_H

#include "ARawSocket.h"
#include "RawData.h"

#include "NetUtils.h"

namespace Net {

class LinuxRawSocket : public ARawSocket {
    static constexpr size_t buffSize = 64 * 1024;

public:
    explicit LinuxRawSocket(Net::Version version, Net::Protocol protocol, std::function<void (data_t const& data)> readCallback);
    virtual ~LinuxRawSocket();

    //! @throw std::system_error
    void startSniffing(std::string const& interface) override;

    int getFd() const { return _fd; }

protected:
    data_t _readSock() const override;

private:
    static uint8_t _sharedBuffer[buffSize];
    int _fd;
};

}

#endif // LINUXRAWSOCKET_H
