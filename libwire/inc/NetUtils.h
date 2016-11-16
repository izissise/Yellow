#ifndef NETUTILS_H
#define NETUTILS_H

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "NetInterfaceInfo.h"
#include "ARawSocket.h"
#include "ANetwork.hpp"
#include "NetType.h"

namespace Net {

//! @throw std::system_error
std::vector<Net::InterfaceInfo> listInterfaces();

}

#endif // NETUTILS_H
