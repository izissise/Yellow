#include "PcapFile.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <ostream>
#include <ostream>
#include <iterator>
#include <stdexcept>
#include <system_error>

#include "Utils.h"

namespace Net {

PcapFile::PcapFile() {
    _header.magic_number = magic_number;
}

void PcapFile::loadFile(std::string const& filePath) {
    std::ifstream file;
    std::ios_base::iostate exceptionMask = file.exceptions() | std::ios::failbit;

    file.exceptions(exceptionMask);

    file.open(filePath, std::ios::in | std::ios::binary);

    auto hBuff = make_resource([] (size_t size) { return new char[size]; }, [] (char* a) { delete[] a; }, sizeof(_header));

    file.read(hBuff.get(), sizeof(_header));
    _header = *(reinterpret_cast<pcap_hdr_t*>(hBuff.get()));

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

    std::cout << "Loaded " << filePath << "\n"
    << "\tVersion: " << _header.version_major << "." << _header.version_minor << "\n"
    << "\tTimezone: " << _header.thiszone << "\n"
    << "\tAccuracy: " << _header.sigfigs << "\n"
    << "\tMax length: " << _header.snaplen << "\n"
    << "\tLink type: " << _header.network << std::endl;
}

void PcapFile::saveFile(std::string const& filePath) const {
    std::ofstream file;
    std::ios_base::iostate exceptionMask = file.exceptions() | std::ios::failbit;

    file.exceptions(exceptionMask);

    file.open(filePath, std::ios::out | std::ios::binary);

    std::ostream_iterator<std::string> outIt(file);
    outIt = _rawData;
//     std::copy(std::begin(_rawData), std::end(_rawData), outIt);
}

std::tuple<uint16_t, uint16_t> PcapFile::versionNumber() const {
    return std::make_tuple(_header.version_major, _header.version_minor);
};

void  PcapFile::versionNumber(uint16_t major, uint16_t minor) {
  _header.version_major = major;
  _header.version_minor = minor;
}

}
