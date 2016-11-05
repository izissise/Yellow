#ifndef ICMP_H
#define ICMP_H

#include <stdint.h>
#include <asm/types.h>
#include <linux/posix_types.h>
#include <asm/byteorder.h>
#include <netinet/in.h>

#include "Utils.h"

namespace Net {

class Icmp {

public:
    Icmp() = default;
private:
};

}

#endif // ICMP_H
