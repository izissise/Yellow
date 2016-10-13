#ifndef NETUTILS_H
#define NETUTILS_H

#include <vector>

#include "NetInterfaceInfo.h"

namespace Net {

std::vector<Net::InterfaceInfo> listInterfaces();

}

#endif // NETUTILS_H
