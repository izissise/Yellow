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

#include "Utils.h"
#include "NetUtils.h"

namespace Net {

template<class STREAM = std::fstream>
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
    void loadFile(std::string const& filePath);

    //! @throw std::system_error
    void saveFile(std::string const& filePath) const;

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
    std::string _rawData;
};

typedef PcapFile<> PcapFile_t;

template<class STREAM>
void PcapFile<STREAM>::loadFile(std::string const& filePath) {
    STREAM file;
    file.open(filePath, std::ios::in | std::ios::binary);

    file.read(reinterpret_cast<char*>(&_header), sizeof(_header));
    if (file.gcount() != sizeof(_header)) {
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
    _rawData = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    if (file.is_open())
        file.close();
}

template<class STREAM>
void PcapFile<STREAM>::saveFile(std::string const& filePath) const {
    STREAM file;
    file.open(filePath, std::ios::out | std::ios::trunc | std::ios::binary);

    file.write(reinterpret_cast<const char*>(&_header), sizeof(_header));
    std::ostreambuf_iterator<char> outIt(file);
    std::copy(std::begin(_rawData), std::end(_rawData), outIt);

    if (file.is_open())
        file.close();
}

template<class STREAM>
PcapFile<STREAM>::PcapFile()
: _needSwap(false) {
    _header.magic_number = magic_number;
}

template<class STREAM>
std::tuple<uint16_t, uint16_t> PcapFile<STREAM>::versionNumber() const {
    return std::make_tuple(_header.version_major, _header.version_minor);
}

template<class STREAM>
void PcapFile<STREAM>::versionNumber(uint16_t major, uint16_t minor) {
    _header.version_major = major;
    _header.version_minor = minor;
}


}

#endif // PCAPFILE_H
