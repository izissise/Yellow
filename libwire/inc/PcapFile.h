#ifndef PCAPFILE_H
#define PCAPFILE_H

#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <fstream>
#include <ostream>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <system_error>
#include <stdint.h>

#include "Utils.h"
#include "ScopeGuard.h"
#include "NetUtils.h"

#include "PcapPacket.h"

namespace Net {

class PcapFile
{
    static const uint32_t magic_number = 0xa1b2c3d4;

typedef struct pcap_hdr_s {
    uint32_t magic_number;   /* magic number */
    uint16_t version_major;  /* major version number */
    uint16_t version_minor;  /* minor version number */
    int32_t  thiszone;       /* GMT to local correction */
    uint32_t sigfigs;        /* accuracy of timestamps */
    uint32_t snaplen;        /* max length of captured packets, in octets */
    uint32_t network;        /* data link type */
} pcap_hdr_t;

public:
    PcapFile();
    ~PcapFile() = default;

    //! @throw std::system_error
    //! @throw std::runtime_error
    template<class STREAM = std::fstream>
    void loadFile(std::string const& filePath);

    //! @throw std::system_error
    template<class STREAM = std::fstream>
    void saveFile(std::string const& filePath) const;

    void addPacket(Net::PcapPacket packet);

    bool isByteSwap() const { return _needSwap; };

    std::tuple<uint16_t, uint16_t> versionNumber() const;
    void versionNumber(uint16_t major, uint16_t minor);

    int32_t timezone() const { return _header.thiszone; };
    void timezone(int32_t zone) { _header.thiszone = zone; };

    uint32_t accuracy() const { return _header.sigfigs; };
    void accuracy(uint32_t accuracy) { _header.sigfigs = accuracy; };
    uint32_t maxLength() const { return _header.snaplen; };
    void maxLength(uint32_t mLength) { _header.snaplen = mLength; };
    uint32_t linkType() const { return _header.network; };
    void linkType(uint32_t linkType) { _header.network = linkType; };

private:
    bool _needSwap;
    pcap_hdr_t _header;
    std::vector<PcapPacket> _packets;
};

template<class STREAM>
void PcapFile::loadFile(std::string const& filePath) {
    STREAM file;
    ScopeGuard guard([&] () {
        file.close();
    });

    file.open(filePath, std::ios::in | std::ios::binary);

    if (file.read(reinterpret_cast<char*>(&_header), sizeof(_header)).gcount() != sizeof(_header)) {
        throw std::runtime_error("Not a pcap file.");
    }

    if (_header.magic_number == magic_number) {
        _needSwap = false;
    } else if (switchEndianness(_header.magic_number) == magic_number) {
        _needSwap = true;
        _header.magic_number = magic_number;
        _header.version_major = switchEndianness(_header.version_major);
        _header.version_minor = switchEndianness(_header.version_minor);
        _header.thiszone = switchEndianness(_header.thiszone);
        _header.sigfigs = switchEndianness(_header.sigfigs);
        _header.snaplen = switchEndianness(_header.snaplen);
        _header.network = switchEndianness(_header.network);
    } else {
        throw std::runtime_error("Not a pcap file.");
    }
    while (!file.eof()) {
        try {
            _packets.push_back(PcapPacket(file, isByteSwap(), _header.snaplen));
        } catch (WrongSize& e) {
            if ((e.getTypeSize() - e.getMissingSize()) != 0 || !file.eof())
                std::cerr << e.what() << std::endl;
        }
    }
}

template<class STREAM>
void PcapFile::saveFile(std::string const& filePath) const {
    STREAM file;
    ScopeGuard guard([&] () {
        file.close();
    });

    file.open(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

    file.write(reinterpret_cast<const char*>(&_header), sizeof(_header));
    for (auto const& packet : _packets) {
        auto d = packet.getRawData();
        file.write(reinterpret_cast<const char*>(d.data()), d.size());
    }
}

}

#endif // PCAPFILE_H
