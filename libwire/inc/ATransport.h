#ifndef ATRANSPORT_H
#define ATRANSPORT_H

#include "NetUtils.h"

namespace Net {

class ATransport {
public:
    ATransport(uint8_t* data, size_t dataSize);
    virtual ~ATransport() = default;

    virtual Net::Transport type() const { return Net::Transport::UNKNOWN; };

    uint8_t* data() const { return _data; }
    size_t dataSize() const { return _dataSize; }

protected:
    uint8_t* _data;
    size_t   _dataSize;
};

}

#endif // ATRANSPORT_H
