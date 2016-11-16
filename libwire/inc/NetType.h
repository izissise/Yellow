#ifndef NETTYPE_H
#define NETTYPE_H

namespace Net {

enum class Version {
    V4,
    V6,
    UNKNOWN
};

enum class Transport {
    TCP,
    UDP,
    ICMP,
    UNKNOWN
};

}

#endif // NETTYPE_H
