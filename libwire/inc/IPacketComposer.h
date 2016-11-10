#ifndef IPAKETCOMPOSER_H
#define IPAKETCOMPOSER_H

#include "NetUtils.h"

namespace Net {

class IPacketComposer {
public:
    virtual ~IPacketComposer() = default;

protected:
    virtual data_slice_t getHeaderBasePtr() const = 0;
};

}

#endif // ISLICEFINDER_H
