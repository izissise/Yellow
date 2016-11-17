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

//! @throw std::runtime_error
void rawMacAddr(unsigned char buff[6], std::string const& mac);

}

#endif // NETUTILS_H
