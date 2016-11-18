#include <catch.hpp>

#include "Udp.h"

static uint8_t d[] = "\xea\x62\x88\x1b\x00\x6f\x89\x1f\x80\x00\x00\x00\x00\x05\xa9\x37\xd0\xbf\xa5\xc1\x8e\xa5\x48\xf7\x15\xfc\x83\x88\xd7\xf4\xb6\x9d\x61\x17\x9d\xcf\x2e\x02\xf8\x5d\x8a\x0e\x4b\x61\x1e\x4b\x3a\x33\xc6\x50\x7b\xb3\x53\x74\x93\x0c\x67\x16\x19\xd6\xd9\xc9\x30\x3c\x82\xf4\xe1\x1a\xcb\x76\x7b\x80\x67\x90\x6f\x61\x63\x96\x20\x85\x57\xff\x6d\xde\x1b\xa0\x22\xce\x31\xe7\x78\xc7\x15\x0c\xbf\x43\x91\x61\xdd\xf2\x76\x13\x29\x0f\x58\x19\x81\xba\x29\x2c\x99";

TEST_CASE("Udp Headers tests", "[net][Udp][protocol][packet]") {

    SECTION("Data should be the ones provided") {
        data_slice_t data(d, sizeof(d) -1);
        Net::Udp udp(data);

        Net::ATransport* tr = &udp;
        CHECK(tr->type() == Net::Transport::UDP);
        CHECK(udp.dstPort() == 34843);
        CHECK(udp.srcPort() == 60002);
        CHECK(udp.dataSize() == 111);
        CHECK(udp.checksum() == 0x891f);
    }

    SECTION("Should throw when header is not the right size") {
        uint8_t header[] = "\x00\x00\x00\x00\x00";
        data_slice_t data(header, sizeof(header) - 1);
        REQUIRE_THROWS_AS(Net::Udp t(data), WrongSize);
    }

    SECTION("Should set the right values") {
        data_slice_t data(d, sizeof(d) -1);
        Net::Udp udp(data);

        udp.srcPort(7465);
        udp.dataSize(56);
        udp.checksum(0x0965);
        CHECK(udp.srcPort() == 7465);
        CHECK(udp.dstPort() == 34843);
        CHECK(udp.dataSize() == 56);
        CHECK(udp.checksum() == 0x0965);
    }
}
