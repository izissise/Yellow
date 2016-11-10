#include "AppInstance.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "LinuxNetwork.hpp"
#include "LinuxRawSocket.h"

#include "NetUtils.h"

#include "Packet.h"

void packetLogger(data_t const& data) {
    try {
        Net::Packet packet(data);

        auto ethernet = packet.ethernetFrame();
        auto ip = packet.getNetworkLayer();
        auto proto = packet.getTransportLayer();

        std::cout << "Ethernet - Src: " << ethernet->srcAddr() << " Dst: " << ethernet->dstAddr()
        << " " << std::to_string(ethernet->type()) << std::endl;
        std::cout << "Ip - Src: " << ip->srcAddr() << " Dst: " << ip->dstAddr() << " TTL: "
        << ip->hopLimit() << " Version: " << (ip->version() == Net::Version::V6 ? "6" : "4") << std::endl;
        std::cout << "Protocol - : " << (proto->type() == Net::Transport::TCP ? "Tcp" : (proto->type() == Net::Transport::UDP ? "Udp" : (proto->type() == Net::Transport::ICMP) ? "Icmp" : "Unknown")) << std::endl;

        if (proto->type() == Net::Transport::TCP) {
            auto tcpProto = dynamic_cast<Net::Tcp*>(proto);
            std::cout << "Tcp - SrcPort: " << tcpProto->srcPort() << " dstPort: " << tcpProto->dstPort() << " windowsSize: " << tcpProto->window() << " checksum: " << tcpProto->sum() << std::endl;
        }

        if (proto->type() == Net::Transport::UDP) {
            auto udpProto = dynamic_cast<Net::Udp*>(proto);
            std::cout << "Udp - SrcPort: " << udpProto->srcPort() << " dstPort: " << udpProto->dstPort() << " dataSize: " << udpProto->dataSize() << " checksum: " << udpProto->checksum() << std::endl;
        }

        if (proto->type() == Net::Transport::ICMP) {
            auto icmpProto = dynamic_cast<Net::Icmp*>(proto);
            std::cout << "Icmp - Type: " << icmpProto->icmpType() << std::endl;
        }

    } catch (WrongSize e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "------------------------------------------------" << std::endl;
}

void networkSetupAndSniff() {
    std::unique_ptr<Net::ANetwork> net = std::make_unique<Net::LinuxNetwork>();
    auto face = Net::listInterfaces();
    for (auto& i : face){
        std::cout << i.getName() << " " << i.getAddr() << std::endl;
    }

    std::shared_ptr<Net::ARawSocket> rawSock = std::make_shared<Net::LinuxRawSocket>(packetLogger);
    net->registerRawSocket(rawSock);
    rawSock->startSniffing(face[1], true);

    while (1) {
        try {
            net->poll(true);
        } catch (std::exception e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    networkSetupAndSniff();
    if (argv[1] != nullptr && argv[1][0] == 'a') {
        App::Instance app;

        return app.run(argc, argv);
    }
    return 0;
}
