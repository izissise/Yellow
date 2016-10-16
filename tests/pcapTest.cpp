#include <catch.hpp>

#include "PcapFile.h"
#include "FsIOHelper.h"
#include "GlobalFsIOHelper.h"

class pcapIOFixture {
public:
    pcapIOFixture()
    : gfsioh(GlobalFsIOHelper::instance()) {
        _1pcap = false;
        gfsioh.registerPath("1.pcap", [] (std::stringstream& stream) {
            stream << "\xd4\xc3\xb2\xa1\x02\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00 \x00\x00\x04\x00\x01\x00\x00\x00";
        }, [this] (const std::stringstream& stream) {
            (void)stream;
        });
    };

    ~pcapIOFixture() {
        gfsioh.unregisterAll();
    };

protected:
    GlobalFsIOHelper& gfsioh;
    bool _1pcap;
};

TEST_CASE_METHOD(pcapIOFixture, "Pcap file tests", "[net][pcap][io]") {

    SECTION("Load a good magic number") {
        Net::PcapFile<FsIOHelper> file;
std::cout << "yo" << std::endl;
        REQUIRE_NOTHROW(file.loadFile("1.pcap"));
        CHECK(_1pcap);
    }
}
