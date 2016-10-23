#ifndef NETUTILS_H
#define NETUTILS_H

#include <vector>

#include "NetInterfaceInfo.h"

namespace Net {

//! @throw std::system_error
std::vector<Net::InterfaceInfo> listInterfaces();

}

#endif // NETUTILS_H
