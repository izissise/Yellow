#ifndef ATRANSPORT_H
#define ATRANSPORT_H

#include "IPacketComposer.h"

#include "NetUtils.h"

namespace Net {

class ATransport  : public IPacketComposer {
public:
    ATransport() = default;
    virtual ~ATransport() = default;

    virtual Net::Transport type() const { return Net::Transport::UNKNOWN; };

protected:
    virtual data_slice_t getHeaderBasePtr() const = 0;
};

}

#endif // ATRANSPORT_H
