#include "AppInstance.h"

#include <string>
#include <vector>
#include <iostream>

#include "NetUtils.h"

void printIfaInfos(Net::InterfaceInfo const& ifa) {
  std::cout << "Name: " << std::string(ifa.getName()) << "\n"
  << "  Addr: " << std::string(ifa.getAddr()) << "\n"
  << "  Netmask: " << std::string(ifa.getNetmask()) << "\n"
  << "  Broadcast: " << std::string(ifa.getBroadcast()) << std::endl;
}

int main(int argc, char *argv[]) {
    for (auto const& ifa : Net::listInterfaces()) {
        printIfaInfos(ifa);
    }

    return 0;
    App::Instance app;

    return app.run(argc, argv);
}
