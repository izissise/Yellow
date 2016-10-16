#include <catch.hpp>

#include "PcapFile.h"
#include "FsIOHelper.h"
#include "GlobalFsIOHelper.h"

class pcapIOFixture {
public:
    pcapIOFixture()
    : gfsioh(GlobalFsIOHelper::instance()) {
        gfsioh.registerPath("1.pcap", [] (std::stringstream& stream) {
            char data[] = "\xd4\xc3\xb2\xa1\x02\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00 \x00\x00\x04\x00\x01\x00\x00\x00";
            stream.write(data, sizeof(data));
        }, [this] (const std::stringstream& stream) {
            (void)stream;
        });
    };

    ~pcapIOFixture() {
        gfsioh.unregisterAll();
    };

protected:
    GlobalFsIOHelper& gfsioh;
};

TEST_CASE_METHOD(pcapIOFixture, "Pcap file tests", "[net][pcap][io]") {

    SECTION("Load a good magic number") {
        Net::PcapFile<FsIOHelper> pcap;
        REQUIRE_NOTHROW(pcap.loadFile("1.pcap"));
        CHECK(pcap.isByteSwap() == false);
    }
}
