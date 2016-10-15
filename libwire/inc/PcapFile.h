#ifndef PCAPFILE_H
#define PCAPFILE_H

#include <string>
#include <vector>
#include <memory>
#include <tuple>

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
    void loadFile(std::string const& filePath);

    //! @throw std::system_error
    void saveFile(std::string const& filePath) const;

    bool isByteSwap() const { return _header.magic_number == magic_number; };

    std::tuple<uint16_t, uint16_t> versionNumber() const;
    void versionNumber(uint16_t major, uint16_t minor);

    uint16_t timezone() const { return _header.thiszone; };
    void timezone(uint16_t zone) { _header.thiszone = zone; };

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

}

#endif // PCAPFILE_H
