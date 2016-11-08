#ifndef LINUXRAWSOCKET_H
#define LINUXRAWSOCKET_H

#include "ARawSocket.h"
#include "RawData.h"
#include "NetInterfaceInfo.h"

#include "NetUtils.h"

#define ETHER_TYPE	0x0800

namespace Net {

class LinuxRawSocket : public ARawSocket {
    static constexpr size_t buffSize = 64 * 1024;
    static data_t _sharedBuffer;

public:
    explicit LinuxRawSocket(std::function<void (data_t const& data)> readCallback);
    virtual ~LinuxRawSocket();

    //! @throw std::system_error
    void startSniffing(Net::InterfaceInfo const& interface, bool promiscuous) override;

    int getFd() const { return _fd; }

protected:
    data_t _readSock() const override;

private:
    int _fd;
};

}

#endif // LINUXRAWSOCKET_H
