#ifndef UDP_H
#define UDP_H

#include <iostream>
#include <stdint.h>
#include <asm/types.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>

#include "Utils.h"

namespace Net {

struct udphdr {
    int16_t source;
    int16_t dest;
    int16_t len;
    int16_t check;
};

class Udp {

public:
    explicit Udp(data_t const& buffer);
    void debugDisplay();
    int16_t source() const { return _udpStruct.source; }
    int16_t dest() const { return _udpStruct.dest; }
    int16_t len() const { return _udpStruct.len; }
    int16_t check() const { return _udpStruct.check; }

private:
    struct udphdr _udpStruct;
};

}

#endif // UDP_H
